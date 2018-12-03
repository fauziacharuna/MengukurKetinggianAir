# Mengukur Ketinggian Air
Project pertama untuk belajar IOT dengan MQTT, WebSocket, dan Highcharts

## Skenario IOT

Mengukur ketinggian air pada suatu wadah (ember, bak, dll) kemudian akan tertampil secara realtime pada web server dengan metode pengiriman mqtt
Hasil pengkuran akan mentrigger LED dan Buzzer sebagai alarm jika ada kondisi yang tidak aman. 
### Kebutuhan

Software

```
Arduino IDE 
```
```
Visual Studio Code, Notepad ++ (Editor text untuk source code website)
```
```
Potainer.io (Docker GUI) untuk website
```

### Penjelasan Program 
```
1. Tahap inisialisasi : 
```
A. Import Library :
Pada bagian ini berfungsi untuk mengimport library mqtt client dari #include <PubSubClient.h> agar arduino dapat menjadi subcsriber, publisher. 
Kemudian mengimport library SPI.h agar dapat saling terkomunikasi dengan devices lain. Serta menambahkan library Ethernet untuk dapat menjalankan fungsi ethernet. 

B. Konfigurasi alamat jaringan : 
Memanggil IPAddress fungsi dari class Ethernet untuk mendefinisikan ip address, dns server, gateway dan server mqtt. Pada bagian ini pembagian alamat ip sebagai berikut : 

IP address Arduino : 10.39.52. 66

Dns Server : 10.39.52.66

Gateway : 10.39.52.254

Subnet : 255.255.255.0 (/24)

IP Address Server Mqtt : 10.33.109.37

```
2. Void setup() : 
```
Pada bagian ini meinginisialisasi baud rate dari serial ada 9600 kemudian pemanggilan server dengan client.setServer() serta pemanggilan callback dengan client.setCallback(). 
Kemudian terdapat perintah memulai koneksi dengan Ethernet.begin() terhadap ipaddress, dns server, gateway dan subnet mask. 

```
3. Void reconnect() : 
```
Fungsi ini untuk menghubungkan arduino dengan server. Arduino akan terus mencoba terkoneksi dengan server. Apabila terhubung dengan server, maka client akan mempublish dengan topic “OutTopic” 
dan client juga subscribe dengan dengan “OutTopic” . apabila koneksi gagal maka akan menampilkan state koneksi selama berapa second dan mencoba terhubung lagi dengan server sampai berhasil. 
void callback(char* topic, byte* payload, unsigned int length) : 
Fungsi yang digunakan untuk memaketkan data topic dari mqtt yang mana memasukkan parameter topic, payload, dan panjang data. Yang mana paket data dikonversi menjadi char dan akan ditampung dan dikirimkan pesannya. 
Dari semula dalam byte menjadi character. Kemudian melalui fungsi ini juga akan diinstansiasi ethClient dari library EthernetClient yang kemudian menjadi client dari PubSubClient artinya objek client menjadi Publisher dan subscriber dari mqtt. 

```
4. Void loop() : 
```
Pada bagian ini mengaktifkan fungsi sensor HC-SR04 dengan mengaktifkan pin trigger untuk membangkitkan sinyal ultrasonik kemudian melalui variabel duration menangkap nilai pantul sinyal ultrasonik yang ditembakkan dengan mengaktifkan pin echo dengan pulse.
Kemudian nilai tersebut akan dihitung untuk mengkalkulasi ketinggian air dengan rumusan, nilai ketinggian air= tinggi wadah air - jarak. Yang mana disini ketinggian air diasumsikan dengan gelas air minum dengan volume 220 ml dengan ketinggian 9 cm. Sehingga dimasukkan rumus :
distance = ketinggianWadah-(duration * 0.034 / 2)
Melalui dikali dengan 0.034/2 karena kecepatan gelombang ultrasonik dalam cm/s adalah 0.034 karena terdapat pantulan sehingga terjadi 2 x penembakan sehingga dibagi 2.
Kemudian terdapat pengondisian apabila nilai distance kurang dari 3 maka terdapat peringatan dan agar nilai dapat diikirim melalui protokol mqtt perlu di konversi menjadi char dengan tipe array dengan perintah char buffer[10],  dtostrf(distance ,0, 0, buffer). 
Artinya nilai variabel pennampung pengukuran sensor agar bisa dikirimkan perlu dijadikan satu paket dalam char. 



