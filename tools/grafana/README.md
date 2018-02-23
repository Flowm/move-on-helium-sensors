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

- Clone the move-on_helium repository to `/opt/move-on_helium`
```
sudo mkdir -p /opt/move-on_helium
sudo chown deploy: /opt/move-on_helium
git clone --recurse-submodules git@gitlab.lrz.de:move-on/move-on_helium.git /opt/move-on_helium
```

- Install services and config
```
sudo /opt/move-on_helium/SEN/tools/grafana/setup.sh
```
