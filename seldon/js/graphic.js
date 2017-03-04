(function($, Drupal)
   {
      Drupal.behaviors.searchingcmd = {
         attach: function(context, settings) {
            if (Drupal.settings.seldon != undefined ) {
               var containers = Drupal.settings.seldon.containers;
               //console.log(containers);
               $('#searchingid', context).once('searchingcmd', function () {
                  for (i=0; i< containers.length;i++) {
                     //console.log(" -> " + containers[i].container);
                     $.getJSON('/SeldonSearchingRequest', 
                        //$.getJSON('/SeldonSearchingRequest', 
                        { container: containers[i].container, 
                           probe: containers[i].probe, 
                           url:   containers[i].url, 
                           date:  containers[i].date,
                           level: containers[i].level,
                           days:  containers[i].days,
                        }, 
                        function(data) {
                           percentageasbycountry(data);
                           charta(data);
                           chartb(data);
                           timegraphics(data); 
                           delaygraphics(data); 
                           // statgraphics(data);
                        }
                     );
                  }  
               })

            }
         }
      };

      function percentageasbycountry(data) {
         var cont = "#" + data.information.container;

         $(cont).highcharts({
            title: {text: null},
            subtitle: {text: 'AS per country (%)',style: {color: '#5f97ee',fontWeight: 'bold'}},
            chart: { 
               zoomType: 'xy',
               spacingRight: 20,
               width: 800,
               height:300,
               defaultSeriesType:'area',
               backgroundColor:'transparent', 
            },
            plotOptions: { 
               area: { 
                  stacking: 'percent', lineColor: '#ffffff',lineWidth: 1, marker: { 
                     enabled: false, lineWidth: 1, lineColor: '#ffffff' 
                  }
               },
            },
            xAxis:[{
               type: 'datetime',
               minRange: 5,
               title: {text: 'Date'},
               labels: {style:{color: 'blue'}}
            }],
            yAxis: {title: {text: null},labels:{style:{color: 'blue'}}},
            legend: {
               layout: 'vertical',
               align: 'right',
               verticalAlign: 'top',
               borderWidth:1,
               x: 0, y: 30,
               itemStyle: { fontSize: '10px'}
            },
            tooltip:{
               useHTML: true,
               formatter: function(){
                  var result = '';
                  var probe  = data.information.probe;
                  var fqdn   = data.information.url;
                  var date   = new Date(this.point.x);
                  var url    = 'https://p-trolette.orange-labs.fr/newapi/gethar.php?time=' + (this.point.x/1000) + '&probe=' + probe + '&url=' + fqdn;
                  result += 'as name:<a href=\"' + url + '\" target=\"_blank\">' + this.point.asname + '</a><br/>';
                  result += 'as num: <a href=\"' + url + '\" target=\"_blank\">' + this.point.asnum + '</a><br/>';
                  result += 'bytes:  <a href=\"' + url + '\" target=\"_blank\">' + this.point.y + '</a><br/>';
                  result += 'har:    <a href=\"' + url + '\" target=\"_blank\">' + date + '</a>';
                  return result;
               }
            },
            exporting:{
               buttons:{
                  yourCustomButton:{
                     text: 'Menu',
                     _titleKey: 'yourKey',
                     menuItems:[
                        {
                           text: '% As Per Country',
                           onclick: function(){percentageasbycountry(data);}
                        },
                        {
                           text: '% Prefix Per Country',
                           onclick: function(){percentageprefixbycountry(data);}
                        },
                        {
                           text: '% Prefix Per As Per Country',
                           onclick: function(){percentageprefixasbycountry(data);}
                        },
                        { separator: true, },
                        {
                           text: 'Cumulated Ip Address Per Country',
                           onclick: function(){cumulatedipaddressbycountry(data);}
                        },
                     ],
                  },
                  contextButton:{ enabled:false },
               }
            },
            credits: false,
            series: data.pasc.data ,
         });
      }

      function percentageprefixbycountry(data) {
         var cont = "#" + data.information.container;

         $(cont).highcharts({
            title: {text:null},
            subtitle: {text: 'Prefix per country (%)',style: {color: '#5f97ee',fontWeight: 'bold'}},
            chart: { 
               zoomType: 'xy',
               spacingRight: 20,
               width: 800,
               height:300,
               defaultSeriesType:'area',
               backgroundColor:'transparent', 
            },
            plotOptions: { 
               area: { 
                  stacking: 'percent', lineColor: '#ffffff',lineWidth: 1, marker: { 
                     enabled: false, lineWidth: 1, lineColor: '#ffffff' 
                  }
               }
            },
            xAxis:[{
               type: 'datetime',
               minRange: 5,
               title: {text: 'Date'},
               labels: {style:{color: 'blue'}}
            }],
            yAxis: {title: {text: null},labels:{style:{color: 'blue'}}},
            legend: {
               layout: 'vertical',
               align: 'right',
               verticalAlign: 'top',
               borderWidth:1,
               x: 0, y: 30,
               itemStyle: { fontSize: '10px'}
            },
            exporting:{
               buttons:{
                  yourCustomButton:{
                     text: 'Menu',
                     _titleKey: 'yourKey',
                     menuItems:[
                        {
                           text: '% As Per Country',
                           onclick: function(){percentageasbycountry(data);}
                        },
                        {
                           text: '% Prefix Per Country',
                           onclick: function(){percentageprefixbycountry(data);}
                        },
                        {
                           text: '% Prefix Per As Per Country',
                           onclick: function(){percentageprefixasbycountry(data);}
                        },
                        { separator: true, },
                        {
                           text: 'Cumulated Ip Address Per Country',
                           onclick: function(){cumulatedipaddressbycountry(data);}
                        },
                        { separator: true, },
                     ],
                  },
                  contextButton:{ enabled:false },
               }
            },
            credits: false,

            series: data.pprc.data ,
         });
      }

      function percentageprefixasbycountry(data) {
         var cont = "#" + data.information.container;

         $(cont).highcharts({
            title: {text:null},
            subtitle: {text: 'Prefix per AS per country (%)',style: {color: '#5f97ee',fontWeight: 'bold'}},
            chart: { 
               zoomType: 'xy',
               spacingRight: 20,
               width: 800,
               height:300,
               defaultSeriesType:'area',
               backgroundColor:'transparent', 
            },
            plotOptions: { 
               area: { 
                  stacking: 'percent', lineColor: '#ffffff',lineWidth: 1, marker: { 
                     enabled: false, lineWidth: 1, lineColor: '#ffffff' 
                  }
               }
            },
            xAxis:[{
               type: 'datetime',
               minRange: 5,
               title: {text: 'Date'},
               labels: {style:{color: 'blue'}}
            }],
            yAxis: {title: {text: null},labels:{style:{color: 'blue'}}},
            legend: {
               layout: 'vertical',
               align: 'right',
               verticalAlign: 'top',
               borderWidth:1,
               x: 0, y: 30,
               itemStyle: { fontSize: '10px'}
            },
            exporting:{
               buttons:{
                  yourCustomButton:{
                     text: 'Menu',
                     _titleKey: 'yourKey',
                     menuItems:[
                        {
                           text: '% As Per Country',
                           onclick: function(){percentageasbycountry(data);}
                        },
                        {
                           text: '% Prefix Per Country',
                           onclick: function(){percentageprefixbycountry(data);}
                        },
                        {
                           text: '% Prefix Per As Per Country',
                           onclick: function(){percentageprefixasbycountry(data);}
                        },
                        { separator: true, },
                        {
                           text: 'Cumulated Ip Address Per Country',
                           onclick: function(){cumulatedipaddressbycountry(data);}
                        },
                        { separator: true, },
                     ],
                  },
                  contextButton:{ enabled:false },
               }
            },
            credits: false,

            series: data.ppasc.data ,
         });
      }

      function cumulatedipaddressbycountry(data) {
         var cont = "#" + data.information.container;
         //console.log(data.cipc.data);

         $(cont).highcharts({
            title: {text:null},
            subtitle: {text: 'Cumulated IP Address',style: {color: '#5f97ee',fontWeight: 'bold'}},
            chart: { 
               zoomType: 'xy',
               spacingRight: 20,
               width: 800,
               height:300,
               defaultSeriesType:'area',
               backgroundColor:'transparent' 
            },
            xAxis:[{
               type: 'datetime',
               minRange: 5,
               title: {text: 'Date'},
               labels: {style:{color: 'blue'}}
            }],
            yAxis: {title: {text: null},labels:{style:{color: 'blue'}}},
            legend: {
               layout: 'vertical',
               align: 'right',
               verticalAlign: 'top',
               borderWidth:1,
               x: 0, y: 30,
               itemStyle: { fontSize: '10px'}
            },
            exporting:{
               buttons:{
                  yourCustomButton:{
                     text: 'Menu',
                     _titleKey: 'yourKey',
                     menuItems:[
                        {
                           text: '% As Per Country',
                           onclick: function(){percentageasbycountry(data);}
                        },
                        {
                           text: '% Prefix Per Country',
                           onclick: function(){percentageprefixbycountry(data);}
                        },
                        {
                           text: '% Prefix Per As Per Country',
                           onclick: function(){percentageprefixasbycountry(data);}
                        },
                        { separator: true, },
                        {
                           text: 'Cumulated Ip Address Per Country',
                           onclick: function(){cumulatedipaddressbycountry(data);}
                        },
                        { separator: true, },
                     ],
                  },
                  contextButton:{ enabled:false },
               }
            },
            credits: false,

            series: data.cipc.data ,
         });
      }

      function charta(msg) {
         var cont = "#a" + msg.information.container;
         //var result = msg.gotas;

         var tmp = msg.gotas;
         //console.log(JSON.stringify(tmp, null, 2));

         var data = [];
         var colors = Highcharts.getOptions().colors;
         var count = 0;
         var sum = tmp['sum'];

         for(var country in tmp){
            //console.log(country);
            //console.log(JSON.stringify(tmp[country]));
            //if (country.localeCompare('sum') !== 0) {
            if (country != 'sum') {
               //console.log(JSON.stringify(tmp[country]['color']));
               var value = {};
               (country == '') ? value.name = 'XX' : value.name = country;
               //value.color = '#778899';
               //value.color = colors[count];
               value.color = tmp[country]['color'];
               value.y = Math.round(100*tmp[country]['sum']/sum);
               value.drilldown = {};
               (country == '') ? value.drilldown.name = 'XX' : value.drilldown.name = country;
               //value.drilldown.color = colors[count];
               //value.drilldown.color = tmp[country]['color'];
               var listas = [];
               var dataas = [];
               var colors = [];
               var sumsum = tmp[country]['sum'];
               //console.log(sumsum);
               for(var as in tmp[country]) {

                  //console.log(JSON.stringify(tmp[country][as]['sum']));
                  //console.log(JSON.stringify(tmp[country]['sum']));
                  //if (as.localeCompare('sum') !== 0) {
                  if (as != 'sum' && as != 'color') {
                     //console.log(tmp[country][as]['sum']);
                     listas.push(as);
                     dataas.push(Math.round((100*tmp[country][as]['sum']/sumsum)*(tmp[country]['sum']/sum)));
                     colors.push(tmp[country][as]['color']);
                  }
               }
               //console.log(colors);
               value.drilldown.color = colors[0];
               value.drilldown.categories = listas;
               value.drilldown.data = dataas;

               data.push(value);
               //count++;
            }
         }

         //console.log(JSON.stringify(data, null, 2));

         countryData = [];
         asData = [];

         for(var i=0; i<data.length;i++) {
            countryData.push({
               name: data[i].name,
               y: data[i].y,
               color: data[i].color
            });
            for(var j=0; j<data[i].drilldown.data.length;j++) {
               var brightness = 0.2 - (j / data[i].drilldown.data.length) / data.length;
               asData.push({
                  name: data[i].drilldown.categories[j],
                  y: data[i].drilldown.data[j],
                  //color: Highcharts.Color(data[i].color).brighten(brightness).get()
                  color: Highcharts.Color(data[i].drilldown.color).brighten(brightness).get()
               });
            }
         }

         $(cont).highcharts({
            chart: {
               backgroundColor:'transparent', 
               plotBackgroundColor: null,
               plotBorderWidth: null,
               plotShadow: false,
               marginLeft: 70,
               marginRight: 70,
               width:350,
               height: 300,
            },
            plotOptions: {
               pie: {
                  allowPointSelect: true,
                  cursor: 'pointer',
                  dataLabels: {
                     enabled: true,
                     style: {
                        fontSize: '10px',
                        color: (Highcharts.theme && Highcharts.theme.contrastTextColor) || 'black',
                     }
                  },
                  shadow: false,
                  center: ['50%', '50%'],
               }
            },
            title: {text: null},
            subtitle: {text: 'AS per ContentType',style: {color: '#5f97ee',fontWeight: 'bold'}},
            yAxis: {title:null},
            //yAxis: {title: {text: 'Total prefix per as per contenttype'},labels:{style:{color: 'blue'}}},
            legend: {itemStyle: { fontFamily: 'serif', fontSize: '10px'} },
            tooltip: {valueSuffix: '%'},
            exporting:{buttons:{contextButton:{enabled:false}}},
            credits: false,
            series: [
               {
                  type: 'pie',
                  name: 'Content Type', 
                  data: countryData,
                  dataLabels: {
                     formatter: function () {
                        return this.y > 5 ? this.point.name : null;
                     },
                     color: 'black',
                     distance: -30
                  }
               },
               {
                  type: 'pie',
                  name: 'AS',
                  data: asData,
                  size: '80%',
                  innerSize: '60%',
                  dataLabels: {
                     formatter: function () {
                        return this.y > 1 ? '<b>' + this.point.name + ':</b> ' + this.y + '%' : null;
                     }
                  }
               }
            ] 

         });
      }

      function chartb(msg) {
         var cont = "#b" + msg.information.container;
         //var result = msg.gotas;

         var tmp = msg.gpasc;

         // console.log(JSON.stringify(tmp, null, 2));

         var data = [];
         // var colors = Highcharts.getOptions().colors;
         var colors = [];

         var sum = tmp['sum'];

         for(var country in tmp){
            if (country != 'sum') {
               var value = {};
               (country == '') ? value.name = 'XX' : value.name = country;
               value.color = tmp[country]['color'];
               value.y = Math.round(100*tmp[country]['sum']/sum);
               value.drilldown = {};
               (country == '') ? value.drilldown.name = 'XX' : value.drilldown.name = country;
               //value.drilldown.color = colors[count];
               var listas = [];
               var dataas = [];
               var sumsum = tmp[country]['sum'];
               //console.log(sumsum);
               for(var as in tmp[country]) {
                  //console.log(as);
                  if (as != 'sum' && as != 'color') {
                     listas.push(as);
                     dataas.push(Math.round((100*tmp[country][as].sum/sumsum)*(tmp[country]['sum']/sum)));
                     // colors.push(tmp[country][as]['color']);
                  }
               }
               value.drilldown.categories = listas;
               value.drilldown.data = dataas;
               // value.drilldown.color = colors[0];
               value.drilldown.color = tmp[country]['color'];
               data.push(value);
               //count++;
            }
         }

         // console.log(JSON.stringify(data, null, 5));

         countryData = [];
         asData = [];

         for(var i=0; i<data.length;i++) {
            countryData.push({
               name: data[i].name,
               y: data[i].y,
               color: data[i].color
            });
            for(var j=0; j<data[i].drilldown.data.length;j++) {
               var brightness = 0.2 - (j / data[i].drilldown.data.length) / data.length;
               asData.push({
                  name: data[i].drilldown.categories[j],
                  y: data[i].drilldown.data[j],
                  color: Highcharts.Color(data[i].color).brighten(brightness).get()
                  //color: Highcharts.Color(data[i].drilldown.color).brighten(brightness).get()
               });
            }
         }

         $(cont).highcharts({
            chart: {
               backgroundColor:'transparent', 
               plotBackgroundColor: null,
               plotBorderWidth: null,
               plotShadow: false,
               marginLeft: 70,
               marginRight: 70,
               width:350,
               height: 300,
            },
            plotOptions: {
               pie: {
                  allowPointSelect: true,
                  cursor: 'pointer',
                  dataLabels: {
                     enabled: true,
                     style: {
                        fontSize: '10px',
                        color: (Highcharts.theme && Highcharts.theme.contrastTextColor) || 'black',
                     }
                  },
                  shadow: false,
                  center: ['50%', '50%'],
               }
            },
            title: {text: null},
            subtitle: {text: 'AS per Country',style: {color: '#5f97ee',fontWeight: 'bold'}},
            yAxis: {title:null},
            //yAxis: {title: {text: 'Total prefix per as per contenttype'},labels:{style:{color: 'blue'}}},
            legend: {itemStyle: { fontFamily: 'serif', fontSize: '10px'} },
            tooltip: {valueSuffix: '%'},
            exporting:{buttons:{contextButton:{enabled:false}}},
            credits: false,
            series: [
               {
                  type: 'pie',
                  name: 'Country', 
                  data: countryData,
                  size: '60%',
                  dataLabels: {
                     formatter: function () {
                        return this.y > 5 ? this.point.name : null;
                     },
                     color: 'black',
                     distance: -30
                  }
               },
               {
                  type: 'pie',
                  name: 'AS',
                  data: asData,
                  size: '80%',
                  innerSize: '60%',
                  dataLabels: {
                     formatter: function () {
                        return this.y > 1 ? '<b>' + this.point.name + ':</b> ' + this.y + '%' : null;
                     }
                  }
               }
            ] 

         });
      }

      function timegraphics(data) {
         var cont = "#t" + data.information.container;
         //console.log(data.cipc.data);

         var colors = Highcharts.getOptions().colors;

         $(cont).highcharts({
            title: {text:null},
            subtitle: {text: 'Page Load Time',style: {color: '#5f97ee',fontWeight: 'bold'}},
            chart: { 
               zoomType: 'xy',
               spacingRight: 20,
               width: 800,
               height:300,
               defaultSeriesType:'area',
               backgroundColor:'transparent' 
            },
            xAxis:[{
               type: 'datetime',
               minRange: 5,
               title: {text: 'Date'},
               labels: {style:{color: 'blue'}}
            }],
            //yAxis: {title: {text: null},labels:{style:{color: 'blue'}}},
            yAxis: [{
               labels: { format: '{value}ms' },
               title: { text: 'pageloadtime' },
               // style: { color: colors[1] }
            },{
               labels: { format: '{value}bytes' },
               title: { text: 'cumulated wait time' },
               // style: { color: colors[2] },
               opposite: true
            }],
            legend: {
               layout: 'vertical',
               align: 'right',
               verticalAlign: 'top',
               borderWidth:1,
               x: 0, y: 30,
               itemStyle: { fontSize: '10px'}
            },
            /*legend: {
               enabled: false
            },*/
            exporting:{buttons:{contextButton:{enabled:false}}},
            credits: false,

            series: data.gtc.data ,
         });
      }

      function delaygraphics(data) {
         var cont = "#d" + data.information.container;
         //console.log(data.cipc.data);

         var colors = Highcharts.getOptions().colors;

         $(cont).highcharts({
            title: {text:null},
            subtitle: {text: 'Page Load Time',style: {color: '#5f97ee',fontWeight: 'bold'}},
            chart: { 
               zoomType: 'xy',
               spacingRight: 20,
               width: 800,
               height:300,
               defaultSeriesType:'area',
               backgroundColor:'transparent' 
            },
            xAxis:[{
               type: 'datetime',
               minRange: 5,
               title: {text: 'Date'},
               labels: {style:{color: 'blue'}}
            }],
            //yAxis: {title: {text: null},labels:{style:{color: 'blue'}}},
            yAxis: [{
               labels: { format: '{value}ms' },
               title: { text: 'page delay time' },
               // style: { color: colors[1] }
            },{
               labels: { format: '{value}' },
               title: { text: 'page weight' },
               // style: { color: colors[2] },
               opposite: true
            }],
            legend: {
               layout: 'vertical',
               align: 'right',
               verticalAlign: 'top',
               borderWidth:1,
               x: 0, y: 30,
               itemStyle: { fontSize: '10px'}
            },
            /*legend: {
               enabled: false
            },*/
            exporting:{buttons:{contextButton:{enabled:false}}},
            credits: false,

            series: data.gdc.data ,
         });
      }


      function statgraphics(data) {
         var cont = "#s" + data.information.container;
         console.log(data.stat.data);

         $(cont).highcharts({
            title: {text:null},
            subtitle: {text: 'Normal Distribution ',style: {color: '#5f97ee',fontWeight: 'bold'}},
            chart: { 
               zoomType: 'xy',
               spacingRight: 20,
               width: 800,
               height:300,
               defaultSeriesType:'column',
               backgroundColor:'transparent' 
            },
            /*xAxis: {
               gridLineWidth: 1
            },*/
            xAxis:[{
               type: 'datetime',
               //minRange: 5,
               title: {text: 'Load Time'},
               labels: {style:{color: 'blue'}}
            }],
            //yAxis: {title: {text: "normal"},labels:{style:{color: 'blue'}}},
            yAxis: [{
               labels: { format: '{value}ms' },
               title: { text: 'Time to load' },
               marker: { radius: 1 },
               // style: { color: Highcharts.getOptions().colors[1] }
            }],
            /*legend: {
               layout: 'vertical',
               align: 'right',
               verticalAlign: 'top',
               borderWidth:1,
               x: 0, y: 30,
               itemStyle: { fontSize: '10px'}
            },*/
            legend: {
               enabled: false
            },
            exporting:{buttons:{contextButton:{enabled:false}}},
            credits: false,

            series: data.stat.data ,
         });

      }

   } (jQuery, Drupal));
