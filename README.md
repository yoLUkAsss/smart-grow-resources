# Slow Farming (Context)

En este proyecto se encuentran dockerizadas las siguientes herramientas:

* Influx DB (Time-Series Database)
Se usara para recolectar informacion del entorno

* Broker MQTT (Broker de Mensajeria Lightweight)
Se usara su mecanismo pub/sub para interactuar con los dispositivos




### Pasos previos

* Creacion de carpetas a utilizar

```bash
mkdir envs
mkdir envs/influxdb             # Volume para influxdb
mkdir envs/influxdb/config
mkdir envs/influxdb/data
mkdir envs/mosquitto            # Volume para mosquitto
mkdir envs/mosquitto/config     # Aca volcaremos la configuracion incial del broker
mkdir envs/mosquitto/data
mkdir envs/mosquitto/log
```

* Agregar confiracion de MQTT (mosquitto.conf)

```bash
touch envs/mosquitto/config/mosquitto.conf
```

En el archivo agregar la siguiente confiracion:

```conf
# PORTS
# this will listen for mqtt on tcp
listener 1883
protocol mqtt
allow_anonymous true

# this will expect websockets connections
listener 9001
protocol websockets
socket_domain ipv4
socket_domain ipv6

# Mosquitto >= 2.0 únicamente permite conexiones autenticadas mediante usuario/contraseña
# Permitimos temporalmente las conexiones anónimas para probar el entorno
allow_anonymous true

# PERSISTANCE
persistence true
persistence_file mosquitto.db
persistence_location /mosquitto/data/

# LOGS
log_timestamp_format %Y-%m-%dT%H:%M:%S
log_dest file /mosquitto/log/mosquitto.log
log_type debug
log_type error
log_type warning
log_type notice
log_type information
log_type all
```


### Iniciar Servicios

```bash
cd broker
docker-compose up -d
```


### Sobre Hardware

Se agrega dentro del proyecto versionado el archivo `sensy.ino` el cual posee el codigo a cargar en el microcontrolador esp32.

- Paquetes a utilizar
    * `esp32` de Expressif Systems (version 2.0.11)
    * `Arduino esp32 Boards` de Arduino (version 2.0.13)

- Librarias
    * `Adafruit Unified Sensor` de Adafruit (version 1.1.12)
    * `DHT Sensor Library` de Adafruit (version 1.4.4)
    * `DHT Sensor Library for ESPx` de Adafruit (version 1.1.12)
    * `PubSubClient` de Nick O'Leary (version 2.8)

- Algunas consideraciones
    * Hoy en dia el broker configurado via docker, utilizara la red por default de docker. Es decir, el host al broker es el gateway de la red virtual de docker.

    * Habra que reemplazar los valores correspondientes de:
    
```ino
// Wifi
const char* ssid = "NOMBRE_WIFI";
const char* password = "PASSWORD_WIFI";

// MQTT
const char* mqtt_server_host = "172.17.0.1"; // Docker GATEWAY (default)
const uint16_t mqtt_server_port = 1883;
```
    

#### Recursos Utiles

* Networking

    - https://www.baeldung.com/ops/docker-assign-static-ip-container
    - https://docs.docker.com/engine/reference/commandline/network_create/#bridge-driver-options
    - https://github.com/eclipse/mosquitto/issues/1104
    - https://forums.docker.com/t/how-to-configure-docker-networking-with-docker-compose-to-enable-remote-access-to-docker-container-via-ip/96990/6
    - https://dev.to/abbazs/a-step-by-step-guide-for-starting-a-mosquitto-broker-service-in-a-containers-with-docker-compose-1j8i
