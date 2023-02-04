#!/bin/bash

#print ip and wait 5 sec
ip a
sleep 5

# Enable SSH
sudo systemctl enable ssh
sudo systemctl start ssh

# Disable swap memory
sudo dphys-swapfile swapoff
sudo dphys-swapfile uninstall
sudo update-rc.d dphys-swapfile remove

# Perform a full update
sudo apt-get update
sudo apt-get upgrade -y
sudo apt-get dist-upgrade -y

# Install Docker, Docker Compose, and Docker.io
sudo apt-get install docker.io docker-compose git -y

#install python3 if not already installed
apt-get install python3 -y
pip install tabulate -y
pip install pyserial -y
pip install crcmod -y

# Enable USB and WiFi
sudo echo "dtoverlay=dwc2" >> /boot/config.txt
sudo echo "dwc2" >> /etc/modules
sudo echo "libcomposite" >> /etc/modules

# Optimize settings for Docker CLI
sudo echo "export DOCKER_CLI_EXPERIMENTAL=enabled" >> ~/.bashrc
sudo echo "export DOCKER_HOST=tcp://0.0.0.0:2375" >> ~/.bashrc

# Disable Bluetooth, VNC, Telnet
sudo systemctl disable hciuart
sudo systemctl disable vncserver-x11-serviced.service
sudo systemctl disable telnet.socket

# Update the kernel to the latest version
sudo rpi-update

# USB permission change so docker can access the port.
chmod 777 /dev/ttyUSB0

sudo docker-compose up -d

# Restart Raspberry Pi to apply changes
sudo reboot