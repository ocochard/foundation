#!/usr/bin/env node

process.env.NODE_TLS_REJECT_UNAUTHORIZED = "0";

var Getopt     = require('node-getopt');
var Engine     = require('./engine');



var getopt = new Getopt([
   ['p', 'period=ARG'],
   ['u', 'url=ARG'],
   ['h', 'help'],
   ['v', 'version']
]);

getopt.setHelp(
   "Usage: node hypervisor.js [OPTION]\n" 
   + "\n" 
   + "example: node hypervisor.js --period 600 --url http://192.168.0.2:8080\n" 
   + "\n" 
   + "Options:" 
   + "\n" 
   + "-p, --period <sec>      Maximum delay before to exit\n" 
   + "-u, --url <sec>         The Maas server address \n" 
   + "-h, --help              Show this help\n" 
   + "-v, --version           Version of hypervisor\n" 
   + "\n" 
   + "This program execute a crawl of urls after (s) secondes for (d) secondes\n" 
   + "The time of this program can be over (p) seconds \n"
);

opt = getopt.parse(process.argv.slice(2));
//console.info(opt);

if (!isNaN(opt.options.period)  && typeof opt.options.period   != 'undefined' && typeof opt.options.url    != 'undefined' ) {
   var exit_time    = parseInt(opt.options.period)   * 1000;

   Engine.run( opt.options.url );

   setTimeout(function() {
      console.log('[' + new Date().toISOString() + '] - End of the process');
      process.exit(1);
   }, exit_time);
} else {
   getopt.showHelp();
}
