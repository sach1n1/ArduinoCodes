var mqtt=require('mqtt');
const puppeteer = require('puppeteer');
var topic="Fiware/Test";
var status="close";

options={
    clientId:"mqttjs01",
    username:"openhabian",
    password:"openhabian",
    clean:true};

var client  = mqtt.connect("mqtt://192.168.2.247",options);
console.log("connected flag" + client.connected);

client.on('message',function(topic, message, packet){
    if(topic=="Fiware/Test")
    {

        (async () => {
            if(message=="0")
            {
                await browser.close();
                return;
            }
            status="open";
            const browser = await puppeteer.launch({"headless":false});
            const page = await browser.newPage();
            await page.setViewport({ width: 1280, height: 800 });
            await page.goto('https://www.google.com/');
        })();
    }
});


client.on("connect",function(){	
    console.log("connected  "+ client.connected);    
})
client.subscribe(topic,{qos:0});
