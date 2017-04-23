const http = require('http');
const request = require("request");
const fs = require('fs');

let num = 1;

http.createServer((req, res) => {
  console.log(req.url + " " + req.method);
  console.log("\trequest headers: %j", req.headers);
  let con = request({
    method: req.method,
    url: req.url,
    headers: req.headers
  },(error, response, body) => {
    if (error) {
      console.log("\tError with " + req.url + ":\n\t" + error);
      res.statusCode = 404;
      res.end("Error");
    } else {
      console.log("\tresponse headers: %j", response.headers);
      // console.log(body);
      // res.writeHead(response.statusCode, response.headers);
      // res.end(body);
    }
  });
  req.pipe(con);
  con.pipe(res);
  const name = "xx" + num;
  con.pipe(fs.createWriteStream(name));
  num++;
}).listen(1337);
