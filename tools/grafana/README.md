# Grafana visualization of moveon data

## Setup

### Components
- [Mosquitto](https://github.com/eclipse/mosquitto)
- [InfluxDB](https://github.com/influxdata/influxdb)
- [Telegraf](https://github.com/influxdata/telegraf)
- [Grafana](https://github.com/grafana/grafana)
- [Nginx](https://github.com/nginx/nginx)

### Install
- Automated setup of required software using [piot-ansible](https://github.com/Flowm/piot-ansible):
```
ansible-playbook -i "devpi," devpi.yml
```
- Deploy telegraf config
```
sudo ./setup.sh
```
