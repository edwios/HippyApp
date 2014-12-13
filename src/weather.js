
var selection = 0;
var colors = ["Red","Green","Blue","Cyan","White","Warmwhite","Purple","Magenta","Yellow","Orange","Pink","Oldlace"];

var xhrRequest = function (url, type, body, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  if (type == "POST") {
    xhr.setRequestHeader("Content-type","application/x-www-form-urlencoded");
    xhr.send(body);
  } else {
    xhr.send();
  }
};

function locationSuccess(pos) {
  var dictionary = {};
  // Construct URL
  var url = "https://api.thingspeak.com/channels/15453/feeds/last";
  var url2 = "https://api.thingspeak.com/channels/15455/feeds/last";
  var url3 = "https://api.spark.io/v1/devices/DEVICE_ID/clcolor?access_token=ACCESS_TOKEN";
  
  if (selection === 0) {
    // Send request to Thingspeak
    xhrRequest(url, 'GET', '', 
      function(responseText) {
        // responseText contains a JSON object with weather info
        var json = JSON.parse(responseText);
        var temperature = Math.round(json.field1);
        console.log("Temperature is " + temperature);
        var conditions = Math.round(json.field2 * 10);      
        console.log("Conditions are " + conditions);
        var _lastUpdate = new Date(json.created_at);
        var lastUpdate = _lastUpdate.valueOf();
        var thisMoment = Date.now();
        var noupdate = 0;
        if (thisMoment - lastUpdate > 30*60*1000) {
          noupdate = 1;
//          console.log("No update: ", thisMoment-lastUpdate, "flag: ", noupdate);
        }
        // Assemble dictionary using our keys
        dictionary = {
          "KEY_NOUPDATE": noupdate,
          "KEY_TEMPERATURE": temperature,
          "KEY_CONDITIONS": conditions,
          "KEY_UPDATETYPE": 1
        };
            // Send to Pebble
        Pebble.sendAppMessage(dictionary,
          function(e) {
            console.log("Weather info sent to Pebble successfully!");
          },
          function(e) {
            console.log("Error sending weather info to Pebble!");
          }
        );
      }      
    );

    // Send request to Thingspeak
    xhrRequest(url2, 'GET', '', 
      function(responseText) {
        // responseText contains a JSON object with weather info
        var json = JSON.parse(responseText);
        var room = Math.round(json.field1);
        console.log("Temperature is " + room);
        // Assemble dictionary using our keys
        dictionary.KEY_NOUPDATE = 0;
        dictionary.KEY_ROOM = room;
        dictionary.KEY_UPDATETYPE = 2;
            // Send to Pebble
        Pebble.sendAppMessage(dictionary,
          function(e) {
            console.log("Weather info sent to Pebble successfully!");
          },
          function(e) {
            console.log("Error sending weather info to Pebble!");
          }
        );
      }      
    );
    
    // Send request to SparkCore to read the current Cheerlights color
    xhrRequest(url3, 'GET', '', 
      function(responseText) {
        // responseText contains a JSON object with weather info
        var json = JSON.parse(responseText);
        var clcolor = Math.round(json.result);
        console.log("CheerLights color is " + clcolor);
        // Assemble dictionary using our keys
        if (clcolor > 12) clcolor = 0;
        dictionary.KEY_CLCOLOR = clcolor;
        dictionary.KEY_UPDATETYPE = 3;
            // Send to Pebble
        Pebble.sendAppMessage(dictionary,
          function(e) {
            console.log("CheerLights color sent to Pebble successfully!");
          },
          function(e) {
            console.log("Error sending CheerLights color to Pebble!");
          }
        );
      }      
    );
    
  } else {
    var mesg = "api_key=API_KEY&status=May the world #cheerlights turn " + colors[selection-1]; 
    url = "https://api.thingspeak.com/apps/thingtweet/1/statuses/update";
    // Send request to Thingspeak
    xhrRequest(url, 'POST', mesg, 
      function(responseText) {
        console.log("Cheerslights color set!");
      });
  }
}

function locationError(err) {
  console.log("Error requesting location!");
}

function getWeather() {
  navigator.geolocation.getCurrentPosition(
    locationSuccess,
    locationError,
    {timeout: 15000, maximumAge: 60000}
  );
}


// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    selection = e.payload.KEY_SELECTION;
    console.log("AppMessage received!");
    getWeather();
  }                     
);

// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
    console.log("PebbleKit JS ready!");

    // Get the initial weather
    getWeather();
  }
);
