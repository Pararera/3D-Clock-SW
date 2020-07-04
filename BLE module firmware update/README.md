# UPDATING HM-10 FIRMWARE

This action is optional. Do it only if your HM-10 BLE module doesn't meet firmware version requirement.<br/>
Before updating the firmware, check the chip on HM-10/HM-16 BLE module.
For <b>HM-10</b>, chip must be <b>CC2540</b> or <b>CC2541</b>.
For <b>HM-16</b>, chip must be <b>CC2640</b>.

For <b>HM-16</b> module, please visit <a href="http://www.jnhuamao.cn/download_rom_en.asp?id=1" target="_blank" title="Click to visit Huamao web site">Huamao Download Center</a>.
<br/>
<b>Steps below are for HM-10 BLE module only!</b><br/>

<br/>

### FIRST STEP

Skip this step if HM-10 has HMSoft firmware version V540 or greater.
Follow instructions from the <a href="https://www.youtube.com/watch?v=ez3491-v8Og" title="Youtube video" target="_blank">video</a>.
You will also need Arduino Nano(or other Arduino board), soldering iron and few jumper wires.<br/>
Files you need are in <i>First step</i> folder.

<br/>

### SECOND STEP

For this step you will need <b>FTDI USB to TTL</b> adapter(eg. <i>FT232RL</i>). Adapter must be in <b>3.3V mode</b>, otherwise you will burn the module.<br/>
Connect <b>VCC</b> from the TTL adapter to <b>VCC</b> of the module, do the same for <b>GND</b>. Connect <b>TX</b> from the TTL to <b>RX</b> from the HM-10 and <b>RX</b> from the TTL to <b>TX</b> from the module.<br/>
Send `AT+SBLUP` command to the module. The module is now in update mode. Open <i>Second step</i> folder, launch <i>HMSoft.exe</i>, select COM port and given `.bin` file and press <i>Load Image</i>.<br/>
After update send `AT+RENEW`, then `AT+RESTART`. Check firmware version with `AT+VERS?` command. Baud rate must be <b>115200 bps</b>. Done! The module is ready to use.

