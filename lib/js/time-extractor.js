'use strict';

var fs = require('fs');

var dirs = [
  './../data/genes/',
  './../data/nodes/',
  './../data/procs/',
  './../data/samples/'
];

var obj = { };

for (var index = 0; index < dirs.length; ++index) {
  var dir = dirs[index];
  var subs = fs.readdirSync(dir).map(function(e) {
    return dir + e;
  });
  for (var index2 = 0; index2 < subs.length; ++index2) {
    var sub = subs[index2];
    var files = fs.readdirSync(sub);
    for (var index3 = 0; index3 < files.length; ++index3) {
      var n = files[index3];
      var ext = n.split(/[.]/g);
      ext = ext[ext.length - 1];
      if (ext[0] !== 'e') continue;
      var b = fs.readFileSync(sub + '/' + n).toString();
      var sets = b.split('\n');
      for (var i4 = 0; i4 < sets.length; ++i4) {
        var s = sets[i4];
        if (s.indexOf('real') !== 0) continue;
        var v = s.split('\t')[1];
        if (obj[sub] == null) obj[sub] = [];
        obj[sub].push(v);
      }
    }
  }
}

console.log(obj);
