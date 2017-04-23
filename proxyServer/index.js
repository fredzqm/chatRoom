const http = require('http');
const request = require("request");
const fs = require('fs');

const map = new Map();

http.createServer((req, res) => {
  console.log(req.url + " " + req.method);
  if (map.has(req.url)) {
    console.log("\tretrieved from cache");
    const store = map.get(req.url);
    res.writeHeader(store.statusCode, store.headers);
    res.end(store.body);
  } else {
    console.log("\trequest headers: %j", req.headers);
    const store = {};
    let body = [];
    const request = http.request({
      method: req.method,
      hostname: a[2],
      path: '\/'+a.slice(3).join('\/'),
      headers: req.headers
    }, (response) => {
      store.statusCode = response.statusCode;
      store.headers = response.headers;
      res.writeHeader(response.statusCode, response.headers);
      response.on('data', (chunk)=>{
        body.push(chunk);
        res.write(chunk);
      });
      response.on('end', ()=>{
        store.body = Buffer.concat(body).toString();
        map.set(req.url, store);
        res.end();
      });
    });
    request.on('error', (e) => {
      console.log("\t Error %j", e);
    });
    request.end();
  }
}).listen(1337);
