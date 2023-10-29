

### Broker

```
docker run -d -it --name broker -p 1883:1883 -v ./mosquitto:/mosquitto/  eclipse-mosquitto
```


##### Configuracion

Configuracion base en mosquitto/config/mosquitto.conf


##### Operaciones

* Subscribirse a topico

`docker exec -it mosquitto mosquitto_sub -v -t topico`

* Publicar mensaje en un topico

`docker exec -it mosquitto mosquitto_pub -t topico -m 'Hello World!'`

##### Seguridad

* Generar password

`docker exec -it mosquitto mosquitto_passwd -c /mosquitto/config/mosquitto.passwd admin`