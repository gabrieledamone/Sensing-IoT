# Sensing-IoT
Sensing &amp; IoT Project Code Repository

**Author:** Gabriele D'Amone

**Overview:** This study aims to predict the Relative Indoor Humidity based on external environmental parameters. 

The **data_collection/** directory contains all scripts and data backups used during part 1 of the coursework.

## Dependencies

This subsystem runs using Matlab R2019b, Arduino IDE and Adafruit IO.

## Files Description

To run this code, first ensure you have installed on Matlab the following Toolboxes:

- **Optimisation Toolbox** (Download here https://uk.mathworks.com/products/optimization.html)
- **Global Optimisation Toolbox** (Download here https://uk.mathworks.com/products/global-optimization.html)
- **Curve Fitting Toolbox** (Download here https://uk.mathworks.com/products/curvefitting.html)
- **Symbolic Math Toolbox** (Download here https://uk.mathworks.com/products/symbolic.html)
- **Statistics and Machine Learning Toolbox** (Download here https://uk.mathworks.com/products/statistics.html)
- **Simulink** (Download here https://uk.mathworks.com/products/simulink.html)


Then, please find below an overview of the Matlab files and their content:

- **AirfoilCoefficient.m** Metamodel of equations for Coefficient of Drag and Coefficient of Lift
  - Download **Coefficient.xlsx**
- **Fatigue.m** Metamodel of Force Profile affected by Fatigue
- **FminconInteriorPoint.m** Fmincon Interior Point Algorithm + Global Search 
- **SQPAlgorithm.m** Fmincon Sequential Quadratic Programming Algorithm + Global Search
