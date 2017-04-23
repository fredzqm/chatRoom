var http = require('http'),
    httpProxy = require('http-proxy');
 
var proxy = httpProxy.createProxyServer({
});

var server = http.createServer(function(req, res) {
  console.log(req.url);
  let a = req.url.split('\/');
  req.url = '\/'+a.slice(3).join('\/');
  let target = a.slice(0, 3).join('\/');
  console.log(target);
  proxy.web(req, res, { target: target});
});
 
server.listen(1337);