%% Locate this file to the "logs" folder

clear all; close all;

%% Parsing
str = fileread("17_10_30");
Cstr = strsplit(str, "\n");
n = length(Cstr);
q = zeros(n, 1);
refLevel = zeros(n, 1);
u = zeros(n, 1);
t = zeros(n, 1);
h_arr = zeros(n-1, 1);
j = 0;
for i=1:n
    X = sscanf(string(Cstr(i)), '%f %f %f %f');
    if (length(X)~=4)      
        continue; 
    end
    q(i) = X(1);
    refLevel(i) = X(2);
    u(i) = X(3);
    t(i) = X(4);
    
    if (i>1)
        h_arr(i) = t(i) - t(i-1);
    end
    j = j + 1;
end
% отбросим лишнее
q = q(1:j);
refLevel = refLevel(1:j);
u = u(1:j);
t = t(1:j);

%% Filtration
wpass = 5e-6; % относительная частота среза
q_filt = lowpass(q, wpass);

figure
plot(t, q, t, q_filt, t, u);
title('q, filtered q, u vs time');
legend('q', 'filtered q', 'u');

%% Velocity

h = median(h_arr); % шаг дискретизации
q_d = diff(q_filt)/h;
wpass_v = 1e-6;
q_d_filt = lowpass(q_d, wpass_v);

figure
plot(t(2:end), q_d, t(2:end), q_d_filt);
title('velocity of q vs time');
legend('q_d', 'filtered q_d');

%% Acceleration
q_dd = diff(q_d_filt)/h;
wpass_a = 1e-6;
q_dd_filt = lowpass(q_dd, wpass_a);

figure
plot(t(3:end), q_dd, t(3:end), q_dd_filt);
title('acceleration of q vs time');
legend('q_dd', 'filtered q_dd');

%% Identification

p = Identify(q_filt(3:end), q_d_filt(2:end), u(3:end), q_dd_filt);

function p = Identify(q, q_d, u, q_dd)
    p = pinv([q q_d u])*q_dd;
end