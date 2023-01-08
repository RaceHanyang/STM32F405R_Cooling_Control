# Radiator README

# STM32F405R_Cooling_Control

Main radiator cooling control MCU, STM32F405R, of Clover.

Forked from HAN's main radiator cooling control, authored by 김세웅.

While the code mostly remains the same, this fork aims to...

1. Accommodate updated pinmap.
- The previous repository consists abnormal .ioc pinmap configuration, due to design errors on the circuit discovered after tape-in.
- The circuit mostly remains the same, but the errors mentioned above are fixed, and therefore hardware schematic regarding the pinmap has been updated.
2. Reconfigure DMA settings.
- So that the board can read more than 8 ADC channels.

# Associated JIRA project

[Atlassian](https://racehanyang.atlassian.net/jira/software/projects/RC/settings/boards/2/roadmap)

Should you believe you have access to our issue tacker system, please reach @Terry Yoon to grant a permission.

Or, outsiders, namely other FSAE teams, may request for internal documentations if some conditions are arranged. Please mail @Terry Yoon with some form of your self description.

# Author in Charge

**Terry Yoon - 윤성배**

Team of Electric Control, Race 29th.

📧 *yoonsb@hanyang.ac.kr*

🗃️ *blog.terrysyoon.com*

# Dev Environments

**STM32CubeIDE:** *V1.10.0*

**STM32F405R FW:** *STM32Cube FW_F4 V1.26.2*

Windows 10 PC

*Readme V1.0*

*Dec 28th, 2022*

@Terry Yoon