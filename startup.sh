#!/bin/bash
# Update the system
apt-get update
echo "update1 done"
#install wget
apt-get install wget -y
#install prep for telegraf
wget -q https://repos.influxdata.com/influxdata-archive_compat.key
echo '393e8779c89ac8d958f81f942f9ad7fb82a25e133faddaf92e15b16e6ac9ce4c influxdata-archive_compat.key' | sha256sum -c && cat influxdata-archive_compat.key | gpg --dearmor |  tee /etc/apt/trusted.gpg.d/influxdata-archive_compat.gpg > /dev/null
echo 'deb [signed-by=/etc/apt/trusted.gpg.d/influxdata-archive_compat.gpg] https://repos.influxdata.com/debian stable main' |  tee /etc/apt/sources.list.d/influxdata.list
echo "source added"
#update new lib
apt-get update -y
#install sudo
apt-get install sudo -y
echo "sudo installed"
# Install Telegraf
apt-get install telegraf
echo "telegraf installed"
# Install Python3 and pip
apt-get install python3 -y
apt-get install pip -y
# Install pip packeges for script
pip install tabulate
pip install pyserial
pip install crcmod
echo "pip done"

chmod 777 /dev/$device
usermod -aG sudo telegraf
# Start the Telegraf service
sudo service telegraf start
echo "everything done"
#sudo -u telegraf python3 /home/Elektricity_meter.py
