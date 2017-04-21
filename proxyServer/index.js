const http = require('http');
const request = require("request");

http.createServer((req, res) => {
  console.log(req.url);
  let con = request({
    url: req.url,
    headers : req.headers
  });
  req.pipe(con);
  con.pipe(res);
}).listen(1337);
