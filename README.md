# ServoDrive: GD32F130Fx

本项目是稚晖君的[ElectronBot](https://github.com/peng-zhihui/ElectronBot)中基于STM32F0舵机驱动板对应国产芯片GD32F130Fx系列的替换方案

电路部分可以用原始ElectronBot的方案，芯片替换即可(封装一致，一片价格在5r左右)，可以通过烧录座先下好程序后焊接，后续参数通过I2C修改。

舵机指令部分可以参见以下表格：

![](/Doc/I2C_Protocol.png)

可选的替换芯片:GD32F130F4-16K的FLASH、GD32F130F6-32K的FLASH或者GD32F130F8-64K的FLASH
