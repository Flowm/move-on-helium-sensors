#!/usr/bin/env bash
set -eux

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Telegraf
cp $DIR/config/etc/telegraf/telegraf.d/*.conf /etc/telegraf/telegraf.d/
systemctl restart telegraf

# Influxdb
influx -execute 'CREATE RETENTION POLICY "moveon" ON telegraf DURATION 14d REPLICATION 1 default'

# Influxdb manual config for auth
##/etc/influxdb/influxdb.conf:
##[http]
##   auth-enabled = true
#influx -port 48086 -execute "CREATE USER flow WITH PASSWORD 'REDACTED' WITH ALL PRIVILEGES"
#influx -port 48086 -execute "CREATE USER moveon WITH PASSWORD 'evict-Clad8old'"
#influx -port 48086 -execute "GRANT WRITE ON telegraf TO moveon"
#influx -port 48086 -execute "CREATE USER grafana WITH PASSWORD 'yeah7Evade-beep'"
#influx -port 48086 -execute "GRANT READ ON telegraf TO grafana"
#influx -port 48086 -execute 'CREATE RETENTION POLICY "moveon" ON telegraf DURATION 28d REPLICATION 1 default'


# Bin
cd $DIR/bin/mqttserial && make install
cd $DIR/bin/reset-nucleo && make install
cd $DIR/bin/sensor-parser && make install

# Daemon
$DIR/daemon/ser2mqtt/setup.sh
