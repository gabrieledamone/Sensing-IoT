%% Read Data

IndoorHumidity = xlsread('IndoorHumidity.xlsx', 'B2:B23742');
OutdoorHumidity = xlsread('OutdoorHumidity.xlsx', 'B2:B23742');
OutdoorTemperature = xlsread('OutdoorTemperature.xlsx', 'B2:B23742');
Ventilation = xlsread('GlassDoor.xlsx', 'A2:A54');
Precipitation = xlsread('Precipitation.xlsx', 'B2:B217');
Sunshine = xlsread('Precipitation.xlsx', 'C2:C217');
TimeStepSensors = readtable('TimeStep.xlsx');
TimeStepVentilation = readtable('TimeStepVentilation.xlsx');
TimeHistory = readtable('TimeStepHistory.xlsx');
%%datetime(TimeStep.jointime, 'InputFormat', 'yyyy-mm-dd hh:mm:ss', 'TimeZone', 'UTC')


%% Normalize Data

NIndoorHumidity = normalize(IndoorHumidity);
NOutdoorHumidity = normalize(OutdoorHumidity);
NOutdoorTemperature = normalize(OutdoorTemperature);
NVentilation = normalize(Ventilation);
NPrecipitation = normalize(Precipitation);
NSunshine = normalize(Sunshine);

%% Smooth Noisy Data
SmoothIndoorHumidity = smoothdata(NIndoorHumidity);
SmoothOutdoorHumidity = smoothdata(NOutdoorHumidity);
SmoothOutdoorTemperature = smoothdata(NOutdoorTemperature);
SmoothSunshine = smoothdata(NSunshine);
SmoothPrecipitation = smoothdata(NPrecipitation);
SmoothVentilation = smoothdata(NVentilation);

%% Plot Graph

% Indoor Humidity, Outdoor humidity, Outdoor Temperature, Sunshine and
% Precipitation
figure
plot(TimeStepSensors.TimeStep,SmoothIndoorHumidity)
hold on
plot(TimeStepSensors.TimeStep,SmoothOutdoorHumidity)
hold on
plot(TimeStepSensors.TimeStep,SmoothOutdoorTemperature)
hold on
plot(TimeHistory.TimeStep,SmoothSunshine)
hold on
plot(TimeHistory.TimeStep,SmoothPrecipitation)
hold off

title('Normalized, Smoothened & Interpolated Data')
legend('Indoor Humidity','Outdoor Humidity', 'Outdoor Temperature', 'Sunshine', 'Precipitation')


% Indoor Humidity, Solar Radiation and Precipitation
figure
plot(TimeStepSensors.TimeStep,SmoothIndoorHumidity)
hold on
plot(TimeHistory.TimeStep,SmoothSunshine)
hold on
plot(TimeHistory.TimeStep,SmoothPrecipitation)
hold off
title('Indoor Humidity')

title('Indoor Humidity Variation')
legend('Indoor Humidity','Sunshine duration', 'Precipitation Amount')
%% Correlation Coefficients

% CorrelationCoefficient = corrcoef(NIndoorHumidity,NVentilation)

%% Ventilation 1

InHum1 = xlsread('Ventilation1.xlsx', 'A2:A72');
TimeStep1 = readtable('Ventilation1Time.xlsx');

TimeInterval = linspace(0,32,71);
TimeIntervalgood = TimeInterval';

[Ven1Metamodel4,gof] = fit(TimeIntervalgood,InHum1,'poly4');

figure
plot(Ven1Metamodel4,TimeIntervalgood,InHum1);
Ven1Metamodel4
title('Ventilation 1');

%figure
%plot(TimeStep1.Time,InHum1)

%% Ventilation 2

InHum1 = xlsread('Ventilation1.xlsx', 'B2:B43');

TimeInterval = linspace(0,19,42);
TimeIntervalgood = TimeInterval';

[Ven2Metamodel4,gof] = fit(TimeIntervalgood,InHum1,'poly4');

figure
plot(Ven2Metamodel4,TimeIntervalgood,InHum1);
Ven2Metamodel4
title('Ventilation 2');

%% Ventilation 3

InHum1 = xlsread('Ventilation1.xlsx', 'C2:C55');

TimeInterval = linspace(0,25,54);
TimeIntervalgood = TimeInterval';

[Ven3Metamodel4,gof] = fit(TimeIntervalgood,InHum1,'poly5');

figure
plot(Ven3Metamodel4,TimeIntervalgood,InHum1);
Ven3Metamodel4;
title('Ventilation 3');


%% Ventilation 4

InHum1 = xlsread('Ventilation1.xlsx', 'D2:D112');

TimeInterval = linspace(0,49,111);
TimeIntervalgood = TimeInterval';

[Ven4Metamodel4,gof] = fit(TimeIntervalgood,InHum1,'poly7');

figure
plot(Ven4Metamodel4,TimeIntervalgood,InHum1);
Ven4Metamodel4;
title('Ventilation 4');

%% Interpolation
%First, use interp1 to interpolate both time series to common timestamps(*). 
PrecipitationInterpolated = interp1(TimeHistory.TimeStep,Precipitation,TimeStepSensors.TimeStep);
SunshineInterpolated =  interp1(TimeHistory.TimeStep,Sunshine,TimeStepSensors.TimeStep);
VentilationInterpolated = interp1(TimeStepVentilation.TimeStep,Ventilation,TimeStepSensors.TimeStep);
%% Matrix Scatter Plot

CorrelationData = xlsread('CorrelationData.xlsx');
figure
corrplot(CorrelationData, 'varNames', {'InHum','OutHum','OutTemp','Precip','Sun'})

%% Rain or Sunshine - Indoor Humidity Correlation
Precipitation_IndoorHumidity_Correlation = corrcoef(PrecipitationInterpolated,IndoorHumidity,'rows', 'pairwise');
Precipitation_IndoorHumidity_Correlation

Sunshine_IndoorHumidity_Correlation = corrcoef(SunshineInterpolated,IndoorHumidity, 'rows', 'pairwise');
Sunshine_IndoorHumidity_Correlation

%% Ventilation - Indoor Humidity Correlation
VentilationInterpolated(isnan(VentilationInterpolated)) = 0; % crosscorr does not accept NaNs
figure
[XCF,lags,bounds] = crosscorr(VentilationInterpolated,NIndoorHumidity);
bounds 
crosscorr(VentilationInterpolated,NIndoorHumidity, 'NumLags', 500)
%% Regression Analysis

% Explanatory variables
x1 = PrecipitationInterpolated;
x2 = SunshineInterpolated;
x3 = OutdoorTemperature;
x4 = OutdoorHumidity;
% Response variable
y = IndoorHumidity;
% Put all the explanatory variables (including a constant term) into one matrix
X = [ones(size(x1)) x1 x2 x3 x4];
% Estimate the parameters
[~,~,~,~,stats] = regress(y,X);
b = regress(y,X);

disp(' ')
disp(['Linear Regression Equation = ' num2str(b(1)) ' + ' num2str(b(2)) '*Precipitation ' num2str(b(3)) '*Solar Radiation +' num2str(b(4)) '*Outdoor Temperature + ' num2str(b(5)) '*Outdoor Humidity  '])

%% Linear Regression Accuracy

RegressionFun = (b(1)) + (b(2))*PrecipitationInterpolated(:,1) + (b(3))*SunshineInterpolated(:,1) + (b(4))*OutdoorTemperature(:,1) + (b(5))*OutdoorHumidity(:,1);
plot(TimeStepSensors.TimeStep,RegressionFun)
hold on
plot(TimeStepSensors.TimeStep,IndoorHumidity)
hold off
title('Prediction Algorithm')
legend('Prediction Algorithm','Real Data')

RegressionFun_IndoorHumidity_Correlation = corrcoef(RegressionFun,IndoorHumidity, 'rows', 'pairwise');
RegressionFun_IndoorHumidity_Correlation

RegressionFun(isnan(RegressionFun)) = 0;

[gf] = gfit(IndoorHumidity,RegressionFun,'3') % root mean squarred error (rmse)

%% Non Linear Model

TableRegressionLearner = readtable('TableRegressionLearner.xlsx');