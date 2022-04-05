/*
 * language_data.js
 * ~~~~~~~~~~~~~~~~
 *
 * This script contains the language-specific data used by searchtools.js,
 * namely the list of stopwords, stemmer, scorer and splitter.
 *
 * :copyright: Copyright 2007-2022 by the Sphinx team, see AUTHORS.
 * :license: BSD, see LICENSE for details.
 *
 */

var stopwords = ["a","al","algo","algunas","algunos","ante","antes","como","con","contra","cual","cuando","de","del","desde","donde","durante","e","el","ella","ellas","ellos","en","entre","era","erais","eran","eras","eres","es","esa","esas","ese","eso","esos","esta","estaba","estabais","estaban","estabas","estad","estada","estadas","estado","estados","estamos","estando","estar","estaremos","estar\u00e1","estar\u00e1n","estar\u00e1s","estar\u00e9","estar\u00e9is","estar\u00eda","estar\u00edais","estar\u00edamos","estar\u00edan","estar\u00edas","estas","este","estemos","esto","estos","estoy","estuve","estuviera","estuvierais","estuvieran","estuvieras","estuvieron","estuviese","estuvieseis","estuviesen","estuvieses","estuvimos","estuviste","estuvisteis","estuvi\u00e9ramos","estuvi\u00e9semos","estuvo","est\u00e1","est\u00e1bamos","est\u00e1is","est\u00e1n","est\u00e1s","est\u00e9","est\u00e9is","est\u00e9n","est\u00e9s","fue","fuera","fuerais","fueran","fueras","fueron","fuese","fueseis","fuesen","fueses","fui","fuimos","fuiste","fuisteis","fu\u00e9ramos","fu\u00e9semos","ha","habida","habidas","habido","habidos","habiendo","habremos","habr\u00e1","habr\u00e1n","habr\u00e1s","habr\u00e9","habr\u00e9is","habr\u00eda","habr\u00edais","habr\u00edamos","habr\u00edan","habr\u00edas","hab\u00e9is","hab\u00eda","hab\u00edais","hab\u00edamos","hab\u00edan","hab\u00edas","han","has","hasta","hay","haya","hayamos","hayan","hayas","hay\u00e1is","he","hemos","hube","hubiera","hubierais","hubieran","hubieras","hubieron","hubiese","hubieseis","hubiesen","hubieses","hubimos","hubiste","hubisteis","hubi\u00e9ramos","hubi\u00e9semos","hubo","la","las","le","les","lo","los","me","mi","mis","mucho","muchos","muy","m\u00e1s","m\u00ed","m\u00eda","m\u00edas","m\u00edo","m\u00edos","nada","ni","no","nos","nosotras","nosotros","nuestra","nuestras","nuestro","nuestros","o","os","otra","otras","otro","otros","para","pero","poco","por","porque","que","quien","quienes","qu\u00e9","se","sea","seamos","sean","seas","seremos","ser\u00e1","ser\u00e1n","ser\u00e1s","ser\u00e9","ser\u00e9is","ser\u00eda","ser\u00edais","ser\u00edamos","ser\u00edan","ser\u00edas","se\u00e1is","sido","siendo","sin","sobre","sois","somos","son","soy","su","sus","suya","suyas","suyo","suyos","s\u00ed","tambi\u00e9n","tanto","te","tendremos","tendr\u00e1","tendr\u00e1n","tendr\u00e1s","tendr\u00e9","tendr\u00e9is","tendr\u00eda","tendr\u00edais","tendr\u00edamos","tendr\u00edan","tendr\u00edas","tened","tenemos","tenga","tengamos","tengan","tengas","tengo","teng\u00e1is","tenida","tenidas","tenido","tenidos","teniendo","ten\u00e9is","ten\u00eda","ten\u00edais","ten\u00edamos","ten\u00edan","ten\u00edas","ti","tiene","tienen","tienes","todo","todos","tu","tus","tuve","tuviera","tuvierais","tuvieran","tuvieras","tuvieron","tuviese","tuvieseis","tuviesen","tuvieses","tuvimos","tuviste","tuvisteis","tuvi\u00e9ramos","tuvi\u00e9semos","tuvo","tuya","tuyas","tuyo","tuyos","t\u00fa","un","una","uno","unos","vosotras","vosotros","vuestra","vuestras","vuestro","vuestros","y","ya","yo","\u00e9l","\u00e9ramos"];


/* Non-minified version is copied as a separate JS file, is available */
BaseStemmer=function(){this.setCurrent=function(r){this.current=r;this.cursor=0;this.limit=this.current.length;this.limit_backward=0;this.bra=this.cursor;this.ket=this.limit};this.getCurrent=function(){return this.current};this.copy_from=function(r){this.current=r.current;this.cursor=r.cursor;this.limit=r.limit;this.limit_backward=r.limit_backward;this.bra=r.bra;this.ket=r.ket};this.in_grouping=function(r,t,i){if(this.cursor>=this.limit)return false;var s=this.current.charCodeAt(this.cursor);if(s>i||s<t)return false;s-=t;if((r[s>>>3]&1<<(s&7))==0)return false;this.cursor++;return true};this.in_grouping_b=function(r,t,i){if(this.cursor<=this.limit_backward)return false;var s=this.current.charCodeAt(this.cursor-1);if(s>i||s<t)return false;s-=t;if((r[s>>>3]&1<<(s&7))==0)return false;this.cursor--;return true};this.out_grouping=function(r,t,i){if(this.cursor>=this.limit)return false;var s=this.current.charCodeAt(this.cursor);if(s>i||s<t){this.cursor++;return true}s-=t;if((r[s>>>3]&1<<(s&7))==0){this.cursor++;return true}return false};this.out_grouping_b=function(r,t,i){if(this.cursor<=this.limit_backward)return false;var s=this.current.charCodeAt(this.cursor-1);if(s>i||s<t){this.cursor--;return true}s-=t;if((r[s>>>3]&1<<(s&7))==0){this.cursor--;return true}return false};this.eq_s=function(r){if(this.limit-this.cursor<r.length)return false;if(this.current.slice(this.cursor,this.cursor+r.length)!=r){return false}this.cursor+=r.length;return true};this.eq_s_b=function(r){if(this.cursor-this.limit_backward<r.length)return false;if(this.current.slice(this.cursor-r.length,this.cursor)!=r){return false}this.cursor-=r.length;return true};this.find_among=function(r){var t=0;var i=r.length;var s=this.cursor;var e=this.limit;var h=0;var u=0;var n=false;while(true){var c=t+(i-t>>>1);var a=0;var f=h<u?h:u;var l=r[c];var o;for(o=f;o<l[0].length;o++){if(s+f==e){a=-1;break}a=this.current.charCodeAt(s+f)-l[0].charCodeAt(o);if(a!=0)break;f++}if(a<0){i=c;u=f}else{t=c;h=f}if(i-t<=1){if(t>0)break;if(i==t)break;if(n)break;n=true}}do{var l=r[t];if(h>=l[0].length){this.cursor=s+l[0].length;if(l.length<4)return l[2];var v=l[3](this);this.cursor=s+l[0].length;if(v)return l[2]}t=l[1]}while(t>=0);return 0};this.find_among_b=function(r){var t=0;var i=r.length;var s=this.cursor;var e=this.limit_backward;var h=0;var u=0;var n=false;while(true){var c=t+(i-t>>1);var a=0;var f=h<u?h:u;var l=r[c];var o;for(o=l[0].length-1-f;o>=0;o--){if(s-f==e){a=-1;break}a=this.current.charCodeAt(s-1-f)-l[0].charCodeAt(o);if(a!=0)break;f++}if(a<0){i=c;u=f}else{t=c;h=f}if(i-t<=1){if(t>0)break;if(i==t)break;if(n)break;n=true}}do{var l=r[t];if(h>=l[0].length){this.cursor=s-l[0].length;if(l.length<4)return l[2];var v=l[3](this);this.cursor=s-l[0].length;if(v)return l[2]}t=l[1]}while(t>=0);return 0};this.replace_s=function(r,t,i){var s=i.length-(t-r);this.current=this.current.slice(0,r)+i+this.current.slice(t);this.limit+=s;if(this.cursor>=t)this.cursor+=s;else if(this.cursor>r)this.cursor=r;return s};this.slice_check=function(){if(this.bra<0||this.bra>this.ket||this.ket>this.limit||this.limit>this.current.length){return false}return true};this.slice_from=function(r){var t=false;if(this.slice_check()){this.replace_s(this.bra,this.ket,r);t=true}return t};this.slice_del=function(){return this.slice_from("")};this.insert=function(r,t,i){var s=this.replace_s(r,t,i);if(r<=this.bra)this.bra+=s;if(r<=this.ket)this.ket+=s};this.slice_to=function(){var r="";if(this.slice_check()){r=this.current.slice(this.bra,this.ket)}return r};this.assign_to=function(){return this.current.slice(0,this.limit)}};
SpanishStemmer=function(){var r=new BaseStemmer;var e=[["",-1,6],["á",0,1],["é",0,2],["í",0,3],["ó",0,4],["ú",0,5]];var i=[["la",-1,-1],["sela",0,-1],["le",-1,-1],["me",-1,-1],["se",-1,-1],["lo",-1,-1],["selo",5,-1],["las",-1,-1],["selas",7,-1],["les",-1,-1],["los",-1,-1],["selos",10,-1],["nos",-1,-1]];var a=[["ando",-1,6],["iendo",-1,6],["yendo",-1,7],["ándo",-1,2],["iéndo",-1,1],["ar",-1,6],["er",-1,6],["ir",-1,6],["ár",-1,3],["ér",-1,4],["ír",-1,5]];var s=[["ic",-1,-1],["ad",-1,-1],["os",-1,-1],["iv",-1,1]];var u=[["able",-1,1],["ible",-1,1],["ante",-1,1]];var o=[["ic",-1,1],["abil",-1,1],["iv",-1,1]];var t=[["ica",-1,1],["ancia",-1,2],["encia",-1,5],["adora",-1,2],["osa",-1,1],["ista",-1,1],["iva",-1,9],["anza",-1,1],["logía",-1,3],["idad",-1,8],["able",-1,1],["ible",-1,1],["ante",-1,2],["mente",-1,7],["amente",13,6],["ación",-1,2],["ución",-1,4],["ico",-1,1],["ismo",-1,1],["oso",-1,1],["amiento",-1,1],["imiento",-1,1],["ivo",-1,9],["ador",-1,2],["icas",-1,1],["ancias",-1,2],["encias",-1,5],["adoras",-1,2],["osas",-1,1],["istas",-1,1],["ivas",-1,9],["anzas",-1,1],["logías",-1,3],["idades",-1,8],["ables",-1,1],["ibles",-1,1],["aciones",-1,2],["uciones",-1,4],["adores",-1,2],["antes",-1,2],["icos",-1,1],["ismos",-1,1],["osos",-1,1],["amientos",-1,1],["imientos",-1,1],["ivos",-1,9]];var c=[["ya",-1,1],["ye",-1,1],["yan",-1,1],["yen",-1,1],["yeron",-1,1],["yendo",-1,1],["yo",-1,1],["yas",-1,1],["yes",-1,1],["yais",-1,1],["yamos",-1,1],["yó",-1,1]];var l=[["aba",-1,2],["ada",-1,2],["ida",-1,2],["ara",-1,2],["iera",-1,2],["ía",-1,2],["aría",5,2],["ería",5,2],["iría",5,2],["ad",-1,2],["ed",-1,2],["id",-1,2],["ase",-1,2],["iese",-1,2],["aste",-1,2],["iste",-1,2],["an",-1,2],["aban",16,2],["aran",16,2],["ieran",16,2],["ían",16,2],["arían",20,2],["erían",20,2],["irían",20,2],["en",-1,1],["asen",24,2],["iesen",24,2],["aron",-1,2],["ieron",-1,2],["arán",-1,2],["erán",-1,2],["irán",-1,2],["ado",-1,2],["ido",-1,2],["ando",-1,2],["iendo",-1,2],["ar",-1,2],["er",-1,2],["ir",-1,2],["as",-1,2],["abas",39,2],["adas",39,2],["idas",39,2],["aras",39,2],["ieras",39,2],["ías",39,2],["arías",45,2],["erías",45,2],["irías",45,2],["es",-1,1],["ases",49,2],["ieses",49,2],["abais",-1,2],["arais",-1,2],["ierais",-1,2],["íais",-1,2],["aríais",55,2],["eríais",55,2],["iríais",55,2],["aseis",-1,2],["ieseis",-1,2],["asteis",-1,2],["isteis",-1,2],["áis",-1,2],["éis",-1,1],["aréis",64,2],["eréis",64,2],["iréis",64,2],["ados",-1,2],["idos",-1,2],["amos",-1,2],["ábamos",70,2],["áramos",70,2],["iéramos",70,2],["íamos",70,2],["aríamos",74,2],["eríamos",74,2],["iríamos",74,2],["emos",-1,1],["aremos",78,2],["eremos",78,2],["iremos",78,2],["ásemos",78,2],["iésemos",78,2],["imos",-1,2],["arás",-1,2],["erás",-1,2],["irás",-1,2],["ís",-1,2],["ará",-1,2],["erá",-1,2],["irá",-1,2],["aré",-1,2],["eré",-1,2],["iré",-1,2],["ió",-1,2]];var f=[["a",-1,1],["e",-1,2],["o",-1,1],["os",-1,1],["á",-1,1],["é",-1,2],["í",-1,1],["ó",-1,1]];var n=[17,65,16,0,0,0,0,0,0,0,0,0,0,0,0,0,1,17,4,10];var b=0;var m=0;var k=0;function _(){k=r.limit;m=r.limit;b=r.limit;var e=r.cursor;r:{e:{var i=r.cursor;i:{if(!r.in_grouping(n,97,252)){break i}a:{var a=r.cursor;s:{if(!r.out_grouping(n,97,252)){break s}u:while(true){o:{if(!r.in_grouping(n,97,252)){break o}break u}if(r.cursor>=r.limit){break s}r.cursor++}break a}r.cursor=a;if(!r.in_grouping(n,97,252)){break i}s:while(true){u:{if(!r.out_grouping(n,97,252)){break u}break s}if(r.cursor>=r.limit){break i}r.cursor++}}break e}r.cursor=i;if(!r.out_grouping(n,97,252)){break r}i:{var s=r.cursor;a:{if(!r.out_grouping(n,97,252)){break a}s:while(true){u:{if(!r.in_grouping(n,97,252)){break u}break s}if(r.cursor>=r.limit){break a}r.cursor++}break i}r.cursor=s;if(!r.in_grouping(n,97,252)){break r}if(r.cursor>=r.limit){break r}r.cursor++}}k=r.cursor}r.cursor=e;var u=r.cursor;r:{e:while(true){i:{if(!r.in_grouping(n,97,252)){break i}break e}if(r.cursor>=r.limit){break r}r.cursor++}e:while(true){i:{if(!r.out_grouping(n,97,252)){break i}break e}if(r.cursor>=r.limit){break r}r.cursor++}m=r.cursor;e:while(true){i:{if(!r.in_grouping(n,97,252)){break i}break e}if(r.cursor>=r.limit){break r}r.cursor++}e:while(true){i:{if(!r.out_grouping(n,97,252)){break i}break e}if(r.cursor>=r.limit){break r}r.cursor++}b=r.cursor}r.cursor=u;return true}function d(){var i;while(true){var a=r.cursor;r:{r.bra=r.cursor;i=r.find_among(e);if(i==0){break r}r.ket=r.cursor;switch(i){case 1:if(!r.slice_from("a")){return false}break;case 2:if(!r.slice_from("e")){return false}break;case 3:if(!r.slice_from("i")){return false}break;case 4:if(!r.slice_from("o")){return false}break;case 5:if(!r.slice_from("u")){return false}break;case 6:if(r.cursor>=r.limit){break r}r.cursor++;break}continue}r.cursor=a;break}return true}function v(){if(!(k<=r.cursor)){return false}return true}function g(){if(!(m<=r.cursor)){return false}return true}function w(){if(!(b<=r.cursor)){return false}return true}function h(){var e;r.ket=r.cursor;if(r.find_among_b(i)==0){return false}r.bra=r.cursor;e=r.find_among_b(a);if(e==0){return false}if(!v()){return false}switch(e){case 1:r.bra=r.cursor;if(!r.slice_from("iendo")){return false}break;case 2:r.bra=r.cursor;if(!r.slice_from("ando")){return false}break;case 3:r.bra=r.cursor;if(!r.slice_from("ar")){return false}break;case 4:r.bra=r.cursor;if(!r.slice_from("er")){return false}break;case 5:r.bra=r.cursor;if(!r.slice_from("ir")){return false}break;case 6:if(!r.slice_del()){return false}break;case 7:if(!r.eq_s_b("u")){return false}if(!r.slice_del()){return false}break}return true}function p(){var e;r.ket=r.cursor;e=r.find_among_b(t);if(e==0){return false}r.bra=r.cursor;switch(e){case 1:if(!w()){return false}if(!r.slice_del()){return false}break;case 2:if(!w()){return false}if(!r.slice_del()){return false}var i=r.limit-r.cursor;r:{r.ket=r.cursor;if(!r.eq_s_b("ic")){r.cursor=r.limit-i;break r}r.bra=r.cursor;if(!w()){r.cursor=r.limit-i;break r}if(!r.slice_del()){return false}}break;case 3:if(!w()){return false}if(!r.slice_from("log")){return false}break;case 4:if(!w()){return false}if(!r.slice_from("u")){return false}break;case 5:if(!w()){return false}if(!r.slice_from("ente")){return false}break;case 6:if(!g()){return false}if(!r.slice_del()){return false}var a=r.limit-r.cursor;r:{r.ket=r.cursor;e=r.find_among_b(s);if(e==0){r.cursor=r.limit-a;break r}r.bra=r.cursor;if(!w()){r.cursor=r.limit-a;break r}if(!r.slice_del()){return false}switch(e){case 1:r.ket=r.cursor;if(!r.eq_s_b("at")){r.cursor=r.limit-a;break r}r.bra=r.cursor;if(!w()){r.cursor=r.limit-a;break r}if(!r.slice_del()){return false}break}}break;case 7:if(!w()){return false}if(!r.slice_del()){return false}var c=r.limit-r.cursor;r:{r.ket=r.cursor;if(r.find_among_b(u)==0){r.cursor=r.limit-c;break r}r.bra=r.cursor;if(!w()){r.cursor=r.limit-c;break r}if(!r.slice_del()){return false}}break;case 8:if(!w()){return false}if(!r.slice_del()){return false}var l=r.limit-r.cursor;r:{r.ket=r.cursor;if(r.find_among_b(o)==0){r.cursor=r.limit-l;break r}r.bra=r.cursor;if(!w()){r.cursor=r.limit-l;break r}if(!r.slice_del()){return false}}break;case 9:if(!w()){return false}if(!r.slice_del()){return false}var f=r.limit-r.cursor;r:{r.ket=r.cursor;if(!r.eq_s_b("at")){r.cursor=r.limit-f;break r}r.bra=r.cursor;if(!w()){r.cursor=r.limit-f;break r}if(!r.slice_del()){return false}}break}return true}function y(){if(r.cursor<k){return false}var e=r.limit_backward;r.limit_backward=k;r.ket=r.cursor;if(r.find_among_b(c)==0){r.limit_backward=e;return false}r.bra=r.cursor;r.limit_backward=e;if(!r.eq_s_b("u")){return false}if(!r.slice_del()){return false}return true}function q(){var e;if(r.cursor<k){return false}var i=r.limit_backward;r.limit_backward=k;r.ket=r.cursor;e=r.find_among_b(l);if(e==0){r.limit_backward=i;return false}r.bra=r.cursor;r.limit_backward=i;switch(e){case 1:var a=r.limit-r.cursor;r:{if(!r.eq_s_b("u")){r.cursor=r.limit-a;break r}var s=r.limit-r.cursor;if(!r.eq_s_b("g")){r.cursor=r.limit-a;break r}r.cursor=r.limit-s}r.bra=r.cursor;if(!r.slice_del()){return false}break;case 2:if(!r.slice_del()){return false}break}return true}function S(){var e;r.ket=r.cursor;e=r.find_among_b(f);if(e==0){return false}r.bra=r.cursor;switch(e){case 1:if(!v()){return false}if(!r.slice_del()){return false}break;case 2:if(!v()){return false}if(!r.slice_del()){return false}var i=r.limit-r.cursor;r:{r.ket=r.cursor;if(!r.eq_s_b("u")){r.cursor=r.limit-i;break r}r.bra=r.cursor;var a=r.limit-r.cursor;if(!r.eq_s_b("g")){r.cursor=r.limit-i;break r}r.cursor=r.limit-a;if(!v()){r.cursor=r.limit-i;break r}if(!r.slice_del()){return false}}break}return true}this.stem=function(){_();r.limit_backward=r.cursor;r.cursor=r.limit;var e=r.limit-r.cursor;h();r.cursor=r.limit-e;var i=r.limit-r.cursor;r:{e:{var a=r.limit-r.cursor;i:{if(!p()){break i}break e}r.cursor=r.limit-a;i:{if(!y()){break i}break e}r.cursor=r.limit-a;if(!q()){break r}}}r.cursor=r.limit-i;var s=r.limit-r.cursor;S();r.cursor=r.limit-s;r.cursor=r.limit_backward;var u=r.cursor;d();r.cursor=u;return true};this["stemWord"]=function(e){r.setCurrent(e);this.stem();return r.getCurrent()}};
Stemmer = SpanishStemmer;



var splitChars = (function() {
    var result = {};
    var singles = [96, 180, 187, 191, 215, 247, 749, 885, 903, 907, 909, 930, 1014, 1648,
         1748, 1809, 2416, 2473, 2481, 2526, 2601, 2609, 2612, 2615, 2653, 2702,
         2706, 2729, 2737, 2740, 2857, 2865, 2868, 2910, 2928, 2948, 2961, 2971,
         2973, 3085, 3089, 3113, 3124, 3213, 3217, 3241, 3252, 3295, 3341, 3345,
         3369, 3506, 3516, 3633, 3715, 3721, 3736, 3744, 3748, 3750, 3756, 3761,
         3781, 3912, 4239, 4347, 4681, 4695, 4697, 4745, 4785, 4799, 4801, 4823,
         4881, 5760, 5901, 5997, 6313, 7405, 8024, 8026, 8028, 8030, 8117, 8125,
         8133, 8181, 8468, 8485, 8487, 8489, 8494, 8527, 11311, 11359, 11687, 11695,
         11703, 11711, 11719, 11727, 11735, 12448, 12539, 43010, 43014, 43019, 43587,
         43696, 43713, 64286, 64297, 64311, 64317, 64319, 64322, 64325, 65141];
    var i, j, start, end;
    for (i = 0; i < singles.length; i++) {
        result[singles[i]] = true;
    }
    var ranges = [[0, 47], [58, 64], [91, 94], [123, 169], [171, 177], [182, 184], [706, 709],
         [722, 735], [741, 747], [751, 879], [888, 889], [894, 901], [1154, 1161],
         [1318, 1328], [1367, 1368], [1370, 1376], [1416, 1487], [1515, 1519], [1523, 1568],
         [1611, 1631], [1642, 1645], [1750, 1764], [1767, 1773], [1789, 1790], [1792, 1807],
         [1840, 1868], [1958, 1968], [1970, 1983], [2027, 2035], [2038, 2041], [2043, 2047],
         [2070, 2073], [2075, 2083], [2085, 2087], [2089, 2307], [2362, 2364], [2366, 2383],
         [2385, 2391], [2402, 2405], [2419, 2424], [2432, 2436], [2445, 2446], [2449, 2450],
         [2483, 2485], [2490, 2492], [2494, 2509], [2511, 2523], [2530, 2533], [2546, 2547],
         [2554, 2564], [2571, 2574], [2577, 2578], [2618, 2648], [2655, 2661], [2672, 2673],
         [2677, 2692], [2746, 2748], [2750, 2767], [2769, 2783], [2786, 2789], [2800, 2820],
         [2829, 2830], [2833, 2834], [2874, 2876], [2878, 2907], [2914, 2917], [2930, 2946],
         [2955, 2957], [2966, 2968], [2976, 2978], [2981, 2983], [2987, 2989], [3002, 3023],
         [3025, 3045], [3059, 3076], [3130, 3132], [3134, 3159], [3162, 3167], [3170, 3173],
         [3184, 3191], [3199, 3204], [3258, 3260], [3262, 3293], [3298, 3301], [3312, 3332],
         [3386, 3388], [3390, 3423], [3426, 3429], [3446, 3449], [3456, 3460], [3479, 3481],
         [3518, 3519], [3527, 3584], [3636, 3647], [3655, 3663], [3674, 3712], [3717, 3718],
         [3723, 3724], [3726, 3731], [3752, 3753], [3764, 3772], [3774, 3775], [3783, 3791],
         [3802, 3803], [3806, 3839], [3841, 3871], [3892, 3903], [3949, 3975], [3980, 4095],
         [4139, 4158], [4170, 4175], [4182, 4185], [4190, 4192], [4194, 4196], [4199, 4205],
         [4209, 4212], [4226, 4237], [4250, 4255], [4294, 4303], [4349, 4351], [4686, 4687],
         [4702, 4703], [4750, 4751], [4790, 4791], [4806, 4807], [4886, 4887], [4955, 4968],
         [4989, 4991], [5008, 5023], [5109, 5120], [5741, 5742], [5787, 5791], [5867, 5869],
         [5873, 5887], [5906, 5919], [5938, 5951], [5970, 5983], [6001, 6015], [6068, 6102],
         [6104, 6107], [6109, 6111], [6122, 6127], [6138, 6159], [6170, 6175], [6264, 6271],
         [6315, 6319], [6390, 6399], [6429, 6469], [6510, 6511], [6517, 6527], [6572, 6592],
         [6600, 6607], [6619, 6655], [6679, 6687], [6741, 6783], [6794, 6799], [6810, 6822],
         [6824, 6916], [6964, 6980], [6988, 6991], [7002, 7042], [7073, 7085], [7098, 7167],
         [7204, 7231], [7242, 7244], [7294, 7400], [7410, 7423], [7616, 7679], [7958, 7959],
         [7966, 7967], [8006, 8007], [8014, 8015], [8062, 8063], [8127, 8129], [8141, 8143],
         [8148, 8149], [8156, 8159], [8173, 8177], [8189, 8303], [8306, 8307], [8314, 8318],
         [8330, 8335], [8341, 8449], [8451, 8454], [8456, 8457], [8470, 8472], [8478, 8483],
         [8506, 8507], [8512, 8516], [8522, 8525], [8586, 9311], [9372, 9449], [9472, 10101],
         [10132, 11263], [11493, 11498], [11503, 11516], [11518, 11519], [11558, 11567],
         [11622, 11630], [11632, 11647], [11671, 11679], [11743, 11822], [11824, 12292],
         [12296, 12320], [12330, 12336], [12342, 12343], [12349, 12352], [12439, 12444],
         [12544, 12548], [12590, 12592], [12687, 12689], [12694, 12703], [12728, 12783],
         [12800, 12831], [12842, 12880], [12896, 12927], [12938, 12976], [12992, 13311],
         [19894, 19967], [40908, 40959], [42125, 42191], [42238, 42239], [42509, 42511],
         [42540, 42559], [42592, 42593], [42607, 42622], [42648, 42655], [42736, 42774],
         [42784, 42785], [42889, 42890], [42893, 43002], [43043, 43055], [43062, 43071],
         [43124, 43137], [43188, 43215], [43226, 43249], [43256, 43258], [43260, 43263],
         [43302, 43311], [43335, 43359], [43389, 43395], [43443, 43470], [43482, 43519],
         [43561, 43583], [43596, 43599], [43610, 43615], [43639, 43641], [43643, 43647],
         [43698, 43700], [43703, 43704], [43710, 43711], [43715, 43738], [43742, 43967],
         [44003, 44015], [44026, 44031], [55204, 55215], [55239, 55242], [55292, 55295],
         [57344, 63743], [64046, 64047], [64110, 64111], [64218, 64255], [64263, 64274],
         [64280, 64284], [64434, 64466], [64830, 64847], [64912, 64913], [64968, 65007],
         [65020, 65135], [65277, 65295], [65306, 65312], [65339, 65344], [65371, 65381],
         [65471, 65473], [65480, 65481], [65488, 65489], [65496, 65497]];
    for (i = 0; i < ranges.length; i++) {
        start = ranges[i][0];
        end = ranges[i][1];
        for (j = start; j <= end; j++) {
            result[j] = true;
        }
    }
    return result;
})();

function splitQuery(query) {
    var result = [];
    var start = -1;
    for (var i = 0; i < query.length; i++) {
        if (splitChars[query.charCodeAt(i)]) {
            if (start !== -1) {
                result.push(query.slice(start, i));
                start = -1;
            }
        } else if (start === -1) {
            start = i;
        }
    }
    if (start !== -1) {
        result.push(query.slice(start));
    }
    return result;
}


