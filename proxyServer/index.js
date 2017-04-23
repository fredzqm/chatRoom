const http = require('http');
const request = require("request");
const fs = require('fs');

http.createServer((req, res) => {
  console.log(req.url + " " + req.method);
  console.log("\trequest headers: %j", req.headers);
  let a = req.url.split('\/');
  const request = http.request({
    protocol: 'http:',
    method: req.method,
    hostname: a[2],
    path: '\/'+a.slice(3).join('\/'),
    headers: req.headers
  }, (response) => {
    res.writeHeader(response.statusCode, response.headers);
    response.on('data', (chunk)=>{
      res.write(chunk);
    });
    response.on('end', ()=>{
      res.end();
    });
  });
  request.on('error', (e) => {
    console.log("\t Error %j", e);
  });
  request.end();
}).listen(1337);
