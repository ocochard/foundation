var http       = require('http');
var os         = require('os')
var util       = require('util');
var url        = require('url');

var Docker     = require('dockerode');

var socket = '/var/run/docker.sock';
var docker = new Docker({socketPath: socket});

exports.run = function( url ) {
   process(url);

   pullimage(url);
}

function pullimage( _url ) {
  var hyper = { name: os.hostname(), };

  var hyperString = JSON.stringify(hyper);

   var headers = {
      'Content-Type': 'application/json',
      'Content-Length': Buffer.byteLength(hyperString)
   };

   var options = {
      hostname  : url.parse(_url).hostname,
      //hostname  : 'maas',
      port      : url.parse(_url).port,
      //port      : 8080,
      path      : '/getimages.php',
      method    : 'POST',
      headers   : headers
   };

   console.log("# " + JSON.stringify(hyper));
   console.log("# " + JSON.stringify(options));

   var req = http.request(options, function(res) {
      var str = '';

      console.log(`STATUS: ${res.statusCode}`);
      console.log(`HEADERS: ${JSON.stringify(res.headers)}`);

      res.setEncoding('utf8');
      res.on('data', function (chunk) {
         console.log(`BODY: ${chunk}`);
         str += chunk;
      });
      res.on('end', function () {
         //console.log('No more data in response.')
         //console.log(str);
         var jsonObject = JSON.parse(str);
         if (util.isArray(jsonObject.images)) {
            if (jsonObject.images.length > 0) {
               pullimages(jsonObject.images);
            }
         }
      });
   });

   req.on('error', function(e) {
      console.log('Problem to load URLs: \033[1;31m' + e.message + '\033[0m');
   });

   req.write(hyperString);

   req.end();
}

function process( url ) {
   var hyper = { name: os.hostname(), };   

   var hyperString = JSON.stringify(hyper);

   var headers = {
      'Content-Type': 'application/json',
      'Content-Length': Buffer.byteLength(hyperString)
   };

   var options = {
      hostname  : url.parse(_url).hostname,
      port      : url.parse(_url).port,
      path      : '/getinfo.php',
      method    : 'POST',
      headers   : headers
   };

   //console.log("# " + JSON.stringify(hyper));
   //console.log("# " + JSON.stringify(options));

   var req = http.request(options, function(res) {
      var str = '';

      //console.log(`STATUS: ${res.statusCode}`);
      //console.log(`HEADERS: ${JSON.stringify(res.headers)}`);

      res.setEncoding('utf8');
      res.on('data', function (chunk) {
         //console.log(`BODY: ${chunk}`);
         str += chunk;
      });
      res.on('end', function () {
         //console.log('No more data in response.')
         //console.log(str);
         var jsonObject = JSON.parse(str);
         if (util.isArray(jsonObject.containers)) {
            if (jsonObject.containers.length > 0) {
               analyse(jsonObject.containers);
            }
         }
      });
   });

   req.on('error', function(e) {
      //console.log('Problem to load URLs: \033[1;31m' + e.message + '\033[0m');
   });

   req.write(hyperString);

   req.end();
}

function pullimages( images ) {
   images.forEach(function(entry){
      console.log("Image " + entry.imagename);
      docker.pull(entry.imagename, function(err, stream) {
         docker.modem.followProgress(stream, onFinished, onProgress);
         function onFinished(err, output) {};
         function onProgress(event) {};
      });
   });
}

function analyse( containers ) {
   containers.forEach(function(entry){
      if (entry.dockerid == '0') {
         console.log(entry.containerid);
         console.log(entry.imagename);
         console.log(entry.dockerid);

         docker.pull(entry.imagename, function(err, stream) {
            docker.modem.followProgress(stream, onFinished, onProgress);
            function onFinished(err, output) {
               docker.createContainer({ Image: entry.imagename, name: entry.containerid}, function(err, container) {
                  container.start(function (err, data) {
                     container.inspect(function (err, data) {
                        console.log(data.Id);
                        console.log(data.Name);
                        var name = data.Name;
                        name = name.substring(1);
                        console.log(name);
                        update( name, data.Id );
                     });
                  });
               });
            }
            function onProgress(event) {
            } 
         })
      }
   });
}

function update( containerid, dockerid ) {
   var hyper = { name: os.hostname(), containerid: containerid, dockerid: dockerid};   

   var hyperString = JSON.stringify(hyper);

   var headers = {
      'Content-Type': 'application/json',
      'Content-Length': Buffer.byteLength(hyperString)
   };

   var options = {
      hostname  : 'maas',
      port      : 8080,
      path      : '/update.php',
      method    : 'POST',
      headers   : headers
   };

   //console.log("# " + JSON.stringify(hyper));
   //console.log("# " + JSON.stringify(options));

   var req = http.request(options, function(res) {
      var str = '';

      //console.log(`STATUS: ${res.statusCode}`);
      //console.log(`HEADERS: ${JSON.stringify(res.headers)}`);

      res.setEncoding('utf8');
      res.on('data', function (chunk) {
         //console.log(`BODY: ${chunk}`);
         str += chunk;
      });
      res.on('end', function () {
         //console.log('No more data in response.')
         //console.log(str);
      });
   });

   req.on('error', function(e) {
      console.log('Problem to load URLs: \033[1;31m' + e.message + '\033[0m');
   });

   req.write(hyperString);

   req.end();

}

