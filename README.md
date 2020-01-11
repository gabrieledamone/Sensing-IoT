# Sensing-IoT
Sensing &amp; IoT Project Code Repository

**Author:** Gabriele D'Amone

**Overview:** This study aims to predict the Relative Indoor Humidity based on external environmental parameters. 

**Paddling Tecnique**

The data_collection/ directory contains all scripts and data backups used during part 1 of the coursework.

**Paddle Blade Design**

- Blade Cross Section
- Blade Surface Area


## Dependencies

This subsystem runs using Matlab R2019b and Microsoft Excel.

The packages used by this subsystem are listed in the section below.

## Getting Started

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


## Performance

This subsystems code was developed **up to Friday, December 15th**, on a HP running Windows 10 with the following specifications:

| Processor Name: Intel Core i7 | Processor Speed: 2.6 GHz | Number of Processors: 1 | Total Number of Cores: 8

The code was developed **from Monday, December 19th**, on an MSI Laptop running Windows 10.0.1 with the following specifications:

| Processor Name: Intel Core i7 | Processor Speed: 2.6 GHz | Number of Processors: 1 | Total Number of Cores: 4

Execution time is noted at the end of the Matlab output. Last run time was approximately 3.52 seconds for **FminconInteriorPoint.m**, while 3.15 seconds for **SQPAlgorithm.m**.

## Now it's your turn!

Feel free to change the input values in the code for the **Paddler Strength** and the **Paddling Time**, and determine what paddle specifications best fit your needs. 
