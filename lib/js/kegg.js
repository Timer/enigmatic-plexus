var util = require('util'),
  assert = require('assert'),
  request = require('request'),
  _ = require('lodash'),
  async = require('async'),
  cheerio = require('cheerio');
var parseString = require('xml2js').parseString;

function get_kgml(entry, callback) {
  var url = util.format('http://www.kegg.jp/kegg-bin/download?entry=%s&format=kgml', entry);
  request.get(url, function(err, res, b) {
    assert.equal(res.statusCode, 200);
    parseString(b, function(err, result) {
      assert.equal(err, null);
      var entries = result['pathway']['entry'];
      entries = _.map(
        _.filter(entries, function(e) { return e['$']['type'] == 'gene'; }),
        function(o) {
          return { id: o['$']['id'], link: o['$']['link'] };
        }
      );
      callback(entries);
    });
  });
}

function download_kgml(entry, callback) {
  var i = 0;
  var url = util.format('http://www.kegg.jp/kegg-bin/download?entry=%s&format=kgml', entry);
  request.get(url, function(err, res, b) {
    assert.equal(res.statusCode, 200);

    parseString(b, function(err, result) {
      assert.equal(err, null);
      var entries = result['pathway']['entry'];
      var genes = _.map(
        _.filter(entries, function(e) { return e['$']['type'] == 'gene'; }),
        function(o) {
          o['$']['edges'] = [];
          o['$']['index'] = ++i;
          o['$']['graphics'] = (o['graphics']);
          return { id: o['$']['id'], links: o['$']['link'], edges: o['$']['edges'], index: o['$']['index'], graphics: o['$']['graphics'] };
        }
      );
      var groups = _.map(
        _.filter(entries, function(e) { return e['$']['type'] == 'group'; }),
        function(o) {
          return o;
        }
      );
      var relations = result['pathway']['relation'];
      callback(genes, relations, groups);
    });
  });
}

function build_graph(genes, relations, groups) {
  for (let i of relations) {
    var gene = _.findWhere(genes, { id: i['$']['entry1'] })
    if (gene) {
      gene['edges'].push(i['$']['entry2']);
    }
    else if (var group = _.findWhere(groups, { id: i['$']['entry1'] }))
    {
      console.out(group);
    }
  }
}

function get_gene_names(entries) {
  var ids = _.pluck(entries, 'id');
  var links = _.pluck(entries, 'link');
  async.map(links, function(item, callback) {
    request(item, function(err, res, body) {
      assert.equal(res.statusCode, 200);
      callback(null, cheerio.load(body));
    });
  }, function(err, results) {
    var gene_row = function(e) {
      return cheerio(e).find('th').text() == 'Gene name';
    };
    var genes_data = function(o) {
      return cheerio(o).find('td').text().replace('\n', '');
    };
    var genes = function(o) {
      return _.map(_.filter(o('td[class=fr1]').find('tr'), gene_row), genes_data);
    };
    var strs = _.map(results, genes);
    var concat_genes = function(cat, str) {
      if (cat.length > 0) cat = cat.concat(', ');
      return cat.concat(str);
    };
    var merge_arrays = function(arr) {
      return _.reduce(arr, concat_genes, '');
    };
    strs = _.map(strs, merge_arrays);
    //TODO: keep _.zipObject(ids, strs)
    var search_word = function(g) {
      return g.concat('[tiab]');
    };
    strs = _.map(strs, function(str) {
      var genes = _.map(str.split(/[,]\s*/g), search_word);
      return '('.concat(_.reduce(genes, function(running, c) {
        if (running.length > 0) {
          running = running.concat('+OR+');
        }
        return running.concat(c);
      }, '')).concat(')');
    });
    var queries = [];
    for (var i = 0; i < strs.length; ++i) {
      for (var j = i + 1; j < strs.length; ++j) {
        queries.push('(' + strs[i] + '+AND+' + strs[j] + ')');
      }
    }
    console.log(queries);
  });
}
/*
get_kgml('hsa04810', function(links) {
  get_gene_names(links);
});
*/

download_kgml('hsa04010', function(genes, relations, groups) { build_graph(genes, relations, groups); });
