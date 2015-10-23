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
        var _spl = v.split(/[m]/);
        v = Math.round((parseInt(_spl[0]) * 60 + parseFloat(_spl[1])) * 1000) / 1000;
        obj[sub].push(v);
      }
    }
  }
}

var obj2 = { };

for (var key in obj) {
  if (!obj.hasOwnProperty(key)) continue;
  var arr = key.split(/[/]/);
  var new_key = arr[3];
  if (obj2[new_key] == null) obj2[new_key] = {};
  obj2[new_key][arr[4]] = obj[key];
}

var obj3 = { };

for (var key in obj2) {
  if (!obj2.hasOwnProperty(key)) continue;
  var n = { labels: [], data: [], meta: [] };
  var sub = obj2[key];
  for (var key2 in sub) {
    if (!sub.hasOwnProperty(key2)) continue;
    n['labels'].push(key2);
    var data;
    n['data'].push(data = sub[key2]);
    var avg = data.reduce(function(acc, next) {
      return acc + parseFloat(next);
    }, 0) / data.length;
    var sum = 0;
    for (var _i = 0; _i < data.length; ++_i) {
      sum += Math.pow(data[_i] - avg, 2);
    }
    sum /= data.length - 1;
    var sd = Math.sqrt(sum);
    var se = sd / Math.sqrt(data.length);
    n['meta'].push({ average: Math.round(avg * 1000) / 1000, sd: Math.round(sd * 1000) / 1000, tests: data.length, se: Math.round(se * 1000) / 1000 });
  }
  console.log(key, n);
  obj3[key] = n;
}
