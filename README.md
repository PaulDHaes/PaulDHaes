
# Belgium/Nederlands electricity dashboard raspberry pi for dummies

## Prequsits
- Raspberry pi 3b/ 3b+ /4
- Prefeared 16GB or more sd card for storage
- Raspbian OS 64 bit *if prefeared you can use gui version but is not needed.*
- Special RJ11 to usb cable that has converter build in
  [Buy cable](https://webshop.cedel.nl/Slimme-meter-kabel-P1-naar-USB?gclid=Cj0KCQjwkOqZBhDNARIsAACsbfLt-R-eDRTF_N71Z2UZsGzT_Ve1b7lJw5-FMawz6sVlJQ9zLBQNe-MaAux8EALw_wcB)
- Enable the P1 port on the smart meter this can be done normaly on the distributor website.
  
## What does it do?

Data from Elektricity meter on port P1 this sends data every second -> Rapsberry pi USB with special cable -> Telegraf execute script to recieve data and transforms it to json -> Telegraf sends data to influxdb and put in the right bucket -> Grafana exfiltrate data by the use of a influxdb querry.

## Config_pi.sh
- Print ip and wait 5 sec
- The script enables SSH
- Disables swap memmory -> this is for the degradation prevention of the SD card
- Does a full update
- installs all nessesary programs docker, python3 pip and libs for the python script
- checks if wifi and usb is on and give output on what port the USB is on 
  *if USB port is plugged in to the smart meter then it will print which USB your on handy for telegraf*
- Does some optimazation for docker
- Disables the unnescesery stuff like bluetooth ,vnc and telent
- Updates the kernel
- Changes the UBS permission to let the telegraf container read the USB port
- Does docker-compose up command to pull the container and make a config
- Reboots

## Docker-compose
The docker-compose.yml is here to easly install and configure the right containers.
This includes storage options and the network to let the communicate with each other.
#### influxdb container
Influxdb is a Time Series Data Platform in short time based database. We need this to store the data from telegraf. To manage/debug the influxdb it has a visual webapplication that runs on port 8086. This can be accesed by 
``` web
http://[Raspberrypi_IP]:8086
```
#### grafana container
This is a obsevery platform this means that we can observer all our data easily and clean. To access grafana just go to the raspberry pi following by adding port 3000. The username and passoword are already configured in the docker-compose file but it's recommend to changed it. 
#### telegraf container
This is a server based agent that collects and send the data in this case to influxdb. This like all the other container has no webapplication running instead the port 8125 is used to collect the data. This has also mounts the usb port to that container aswel as the python script needed to fetch the data.
#### portainer container
This is docker container manager this is not nescesry to have this but it easy to mannage the containers and restart them. this has a webapplication on port 9000 and a mangement port on 8000 to conect to other portainers. 
## Elektricity_meter.py
This script reads the data from the P1 port of the smart meter and transform it to readable data that can be parsed. The smart meter spits out obiscodes and these needs to linked to what it actual is after that the script reads the data and transforms it to a json like format.

## Install instructions
### Passwords, usernames and databases are defined in the .env file
### Install Config_pi.sh
This script is if you want to start form a fresh install of Raspbian OS.
to install launche the terminal if you use the GUI version and do:
``` bash
git clone [link of repo]
cd [repo]
chmod +x Config_pi.sh
./Config_pi.sh
```

### Edit telegraf.conf file and the Elektricity_meter.py

```telegraf.conf
[[inputs.exec]]
## Dedault ttyUSB0 if USB is pluged into the top left port
  commands = ["cat /dev/ttyUSB0"]
  timeout = "5s"
  data_format = "influx"
```
```Elektricity_meter.py
# Change your serial port here:
serialport = '/dev/ttyUSB0'
```
### Docker compose use
To run the docker container use the following command and wait till done.
``` bash
docker-compose up -d
```

### Grafana setup
Setup grafana so it can querry data from influxdb. This can be done by simply adding a new database and fill in the fields for influxdb2.

## Make it your own
- Edit the telegraf config so you can have more data from maybe other devices of you network.
- Make your own custom grafana dashboards.
- Edit the .env file to set better passwords.

## Custom telegraf container?

## TODO

- [ ] Update docker container
- [ ] Add more functionality
- [ ] Change storrage space issue
- [ ] Add the USB baudrate in script
- [ ] Make own docker container to easly download
- [ ] secure the communication by using SSL
- [ ] auto edit the grafana dashboard to set all the value
- [ ] edit docker network so localhost can be set
