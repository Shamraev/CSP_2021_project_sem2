%% Locate this file to the "logs" folder

clear all; close all;

str = fileread("12_10_58");
Cstr = strsplit(str, "\n");
n = length(Cstr);
q = zeros(n, 1);
refLevel = zeros(n, 1);
for i=1:n
    X = sscanf(string(Cstr(i)), '%f %f');
    q(i) = X(1);
    refLevel(i) = X(2);
end

plot(q);