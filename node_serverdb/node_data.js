var mqtt = require('mqtt');
var Topic = 'ketinggianAir'; //subscribe to all topics
// var Broker_URL = 'mqtt://broker.shiftr.io';
var Broker_URL = 'mqtt://10.33.109.37';

var Database_URL = '127.0.0.1';

var options = {
	clientId: 'MyMQTT',
	port: 1883,
	// username: '0c47ac2f',
	// password: '72bdff0a34c34bfd',
	// keepalive : 60
};

var client = mqtt.connect(Broker_URL, options);
client.on('connect', mqtt_connect);
client.on('reconnect', mqtt_reconnect);
client.on('error', mqtt_error);
client.on('message', mqtt_messsageReceived);
client.on('close', mqtt_close);

function mqtt_connect() {

	client.subscribe(Topic, mqtt_subscribe);
	// console.log("Connecting MQTT");
}

function mqtt_subscribe(err, granted) {
	console.log("Subscribed to " + Topic);
	if (err) {
		console.log(err);
	}
}

function mqtt_reconnect(err) {
	//console.log("Reconnect MQTT");
	//if (err) {console.log(err);}
	client = mqtt.connect(Broker_URL, options);
}

function mqtt_error(err) {
	//console.log("Error!");
	//if (err) {console.log(err);}
}

function after_publish() {
	//do nothing
}

function mqtt_messsageReceived(topic, message, packet) {
	//console.log('Message received = ' + message);
	insert_message(topic, message, packet);
}

function mqtt_close() {
	//console.log("Close MQTT");
}

////////////////////////////////////////////////////
///////////////////// MYSQL ////////////////////////
////////////////////////////////////////////////////
var mysql = require('mysql');

//Create Connection
var connection = mysql.createConnection({
	host: Database_URL,
	user: "datalog1",
	password: "123456",
	database: "data_iot"
});

connection.connect(function (err) {
	if (err) throw err;
	//console.log("Database Connected!");
});

//insert a row into the tbl_messages table
function insert_message(topic, message, packet) {
	var clientID = "client001";
	var sql = "INSERT INTO ?? (??,??,??) VALUES (?,?,?)";
	var params = ['tbl_messages', 'clientID', 'topic', 'message', clientID, topic, message];
	sql = mysql.format(sql, params);
	connection.query(sql, function (error, results) {
		if (error) throw error;
		console.log("1 record inserted");
	});
};
// function insert_message(topic, message, packet) {
// 	// var clientID= "client001";
// 	var sql = "INSERT INTO ?? (??,??,??) VALUES (?,?,?)";
// 	var params = ['tbl_messages', 'clientID', 'topic', 'message', clientID, topic, message];
// 	sql = mysql.format(sql, params);	
// 	connection.query(sql, function (error, results) {
// 		if (error) throw error;
// 		console.log("1 record inserted");
// 	});
// };