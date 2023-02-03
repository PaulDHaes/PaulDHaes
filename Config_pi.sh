#!/bin/bash

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

# Clone the repository and run the docker-compose file
# sudo git clone [REPO_URL]
# cd [REPO_NAME]
sudo docker-compose up -d

# Restart Raspberry Pi to apply changes
sudo reboot