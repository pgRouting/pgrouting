/*
 * This script contains the language-specific data used by searchtools.js,
 * namely the list of stopwords, stemmer, scorer and splitter.
 */

var stopwords = ["a", "al", "algo", "algunas", "algunos", "ante", "antes", "como", "con", "contra", "cual", "cuando", "de", "del", "desde", "donde", "durante", "e", "el", "ella", "ellas", "ellos", "en", "entre", "era", "erais", "eran", "eras", "eres", "es", "esa", "esas", "ese", "eso", "esos", "esta", "estaba", "estabais", "estaban", "estabas", "estad", "estada", "estadas", "estado", "estados", "estamos", "estando", "estar", "estaremos", "estar\u00e1", "estar\u00e1n", "estar\u00e1s", "estar\u00e9", "estar\u00e9is", "estar\u00eda", "estar\u00edais", "estar\u00edamos", "estar\u00edan", "estar\u00edas", "estas", "este", "estemos", "esto", "estos", "estoy", "estuve", "estuviera", "estuvierais", "estuvieran", "estuvieras", "estuvieron", "estuviese", "estuvieseis", "estuviesen", "estuvieses", "estuvimos", "estuviste", "estuvisteis", "estuvi\u00e9ramos", "estuvi\u00e9semos", "estuvo", "est\u00e1", "est\u00e1bamos", "est\u00e1is", "est\u00e1n", "est\u00e1s", "est\u00e9", "est\u00e9is", "est\u00e9n", "est\u00e9s", "fue", "fuera", "fuerais", "fueran", "fueras", "fueron", "fuese", "fueseis", "fuesen", "fueses", "fui", "fuimos", "fuiste", "fuisteis", "fu\u00e9ramos", "fu\u00e9semos", "ha", "habida", "habidas", "habido", "habidos", "habiendo", "habremos", "habr\u00e1", "habr\u00e1n", "habr\u00e1s", "habr\u00e9", "habr\u00e9is", "habr\u00eda", "habr\u00edais", "habr\u00edamos", "habr\u00edan", "habr\u00edas", "hab\u00e9is", "hab\u00eda", "hab\u00edais", "hab\u00edamos", "hab\u00edan", "hab\u00edas", "han", "has", "hasta", "hay", "haya", "hayamos", "hayan", "hayas", "hay\u00e1is", "he", "hemos", "hube", "hubiera", "hubierais", "hubieran", "hubieras", "hubieron", "hubiese", "hubieseis", "hubiesen", "hubieses", "hubimos", "hubiste", "hubisteis", "hubi\u00e9ramos", "hubi\u00e9semos", "hubo", "la", "las", "le", "les", "lo", "los", "me", "mi", "mis", "mucho", "muchos", "muy", "m\u00e1s", "m\u00ed", "m\u00eda", "m\u00edas", "m\u00edo", "m\u00edos", "nada", "ni", "no", "nos", "nosotras", "nosotros", "nuestra", "nuestras", "nuestro", "nuestros", "o", "os", "otra", "otras", "otro", "otros", "para", "pero", "poco", "por", "porque", "que", "quien", "quienes", "qu\u00e9", "se", "sea", "seamos", "sean", "seas", "seremos", "ser\u00e1", "ser\u00e1n", "ser\u00e1s", "ser\u00e9", "ser\u00e9is", "ser\u00eda", "ser\u00edais", "ser\u00edamos", "ser\u00edan", "ser\u00edas", "se\u00e1is", "sido", "siendo", "sin", "sobre", "sois", "somos", "son", "soy", "su", "sus", "suya", "suyas", "suyo", "suyos", "s\u00ed", "tambi\u00e9n", "tanto", "te", "tendremos", "tendr\u00e1", "tendr\u00e1n", "tendr\u00e1s", "tendr\u00e9", "tendr\u00e9is", "tendr\u00eda", "tendr\u00edais", "tendr\u00edamos", "tendr\u00edan", "tendr\u00edas", "tened", "tenemos", "tenga", "tengamos", "tengan", "tengas", "tengo", "teng\u00e1is", "tenida", "tenidas", "tenido", "tenidos", "teniendo", "ten\u00e9is", "ten\u00eda", "ten\u00edais", "ten\u00edamos", "ten\u00edan", "ten\u00edas", "ti", "tiene", "tienen", "tienes", "todo", "todos", "tu", "tus", "tuve", "tuviera", "tuvierais", "tuvieran", "tuvieras", "tuvieron", "tuviese", "tuvieseis", "tuviesen", "tuvieses", "tuvimos", "tuviste", "tuvisteis", "tuvi\u00e9ramos", "tuvi\u00e9semos", "tuvo", "tuya", "tuyas", "tuyo", "tuyos", "t\u00fa", "un", "una", "uno", "unos", "vosotras", "vosotros", "vuestra", "vuestras", "vuestro", "vuestros", "y", "ya", "yo", "\u00e9l", "\u00e9ramos"];


/* Non-minified version is copied as a separate JS file, if available */
/**@constructor*/
BaseStemmer = function() {
    this.setCurrent = function(value) {
        this.current = value;
        this.cursor = 0;
        this.limit = this.current.length;
        this.limit_backward = 0;
        this.bra = this.cursor;
        this.ket = this.limit;
    };

    this.getCurrent = function() {
        return this.current;
    };

    this.copy_from = function(other) {
        this.current          = other.current;
        this.cursor           = other.cursor;
        this.limit            = other.limit;
        this.limit_backward   = other.limit_backward;
        this.bra              = other.bra;
        this.ket              = other.ket;
    };

    this.in_grouping = function(s, min, max) {
        if (this.cursor >= this.limit) return false;
        var ch = this.current.charCodeAt(this.cursor);
        if (ch > max || ch < min) return false;
        ch -= min;
        if ((s[ch >>> 3] & (0x1 << (ch & 0x7))) == 0) return false;
        this.cursor++;
        return true;
    };

    this.in_grouping_b = function(s, min, max) {
        if (this.cursor <= this.limit_backward) return false;
        var ch = this.current.charCodeAt(this.cursor - 1);
        if (ch > max || ch < min) return false;
        ch -= min;
        if ((s[ch >>> 3] & (0x1 << (ch & 0x7))) == 0) return false;
        this.cursor--;
        return true;
    };

    this.out_grouping = function(s, min, max) {
        if (this.cursor >= this.limit) return false;
        var ch = this.current.charCodeAt(this.cursor);
        if (ch > max || ch < min) {
            this.cursor++;
            return true;
        }
        ch -= min;
        if ((s[ch >>> 3] & (0X1 << (ch & 0x7))) == 0) {
            this.cursor++;
            return true;
        }
        return false;
    };

    this.out_grouping_b = function(s, min, max) {
        if (this.cursor <= this.limit_backward) return false;
        var ch = this.current.charCodeAt(this.cursor - 1);
        if (ch > max || ch < min) {
            this.cursor--;
            return true;
        }
        ch -= min;
        if ((s[ch >>> 3] & (0x1 << (ch & 0x7))) == 0) {
            this.cursor--;
            return true;
        }
        return false;
    };

    this.eq_s = function(s)
    {
        if (this.limit - this.cursor < s.length) return false;
        if (this.current.slice(this.cursor, this.cursor + s.length) != s)
        {
            return false;
        }
        this.cursor += s.length;
        return true;
    };

    this.eq_s_b = function(s)
    {
        if (this.cursor - this.limit_backward < s.length) return false;
        if (this.current.slice(this.cursor - s.length, this.cursor) != s)
        {
            return false;
        }
        this.cursor -= s.length;
        return true;
    };

    /** @return {number} */ this.find_among = function(v)
    {
        var i = 0;
        var j = v.length;

        var c = this.cursor;
        var l = this.limit;

        var common_i = 0;
        var common_j = 0;

        var first_key_inspected = false;

        while (true)
        {
            var k = i + ((j - i) >>> 1);
            var diff = 0;
            var common = common_i < common_j ? common_i : common_j; // smaller
            // w[0]: string, w[1]: substring_i, w[2]: result, w[3]: function (optional)
            var w = v[k];
            var i2;
            for (i2 = common; i2 < w[0].length; i2++)
            {
                if (c + common == l)
                {
                    diff = -1;
                    break;
                }
                diff = this.current.charCodeAt(c + common) - w[0].charCodeAt(i2);
                if (diff != 0) break;
                common++;
            }
            if (diff < 0)
            {
                j = k;
                common_j = common;
            }
            else
            {
                i = k;
                common_i = common;
            }
            if (j - i <= 1)
            {
                if (i > 0) break; // v->s has been inspected
                if (j == i) break; // only one item in v

                // - but now we need to go round once more to get
                // v->s inspected. This looks messy, but is actually
                // the optimal approach.

                if (first_key_inspected) break;
                first_key_inspected = true;
            }
        }
        do {
            var w = v[i];
            if (common_i >= w[0].length)
            {
                this.cursor = c + w[0].length;
                if (w.length < 4) return w[2];
                var res = w[3](this);
                this.cursor = c + w[0].length;
                if (res) return w[2];
            }
            i = w[1];
        } while (i >= 0);
        return 0;
    };

    // find_among_b is for backwards processing. Same comments apply
    this.find_among_b = function(v)
    {
        var i = 0;
        var j = v.length

        var c = this.cursor;
        var lb = this.limit_backward;

        var common_i = 0;
        var common_j = 0;

        var first_key_inspected = false;

        while (true)
        {
            var k = i + ((j - i) >> 1);
            var diff = 0;
            var common = common_i < common_j ? common_i : common_j;
            var w = v[k];
            var i2;
            for (i2 = w[0].length - 1 - common; i2 >= 0; i2--)
            {
                if (c - common == lb)
                {
                    diff = -1;
                    break;
                }
                diff = this.current.charCodeAt(c - 1 - common) - w[0].charCodeAt(i2);
                if (diff != 0) break;
                common++;
            }
            if (diff < 0)
            {
                j = k;
                common_j = common;
            }
            else
            {
                i = k;
                common_i = common;
            }
            if (j - i <= 1)
            {
                if (i > 0) break;
                if (j == i) break;
                if (first_key_inspected) break;
                first_key_inspected = true;
            }
        }
        do {
            var w = v[i];
            if (common_i >= w[0].length)
            {
                this.cursor = c - w[0].length;
                if (w.length < 4) return w[2];
                var res = w[3](this);
                this.cursor = c - w[0].length;
                if (res) return w[2];
            }
            i = w[1];
        } while (i >= 0);
        return 0;
    };

    /* to replace chars between c_bra and c_ket in this.current by the
     * chars in s.
     */
    this.replace_s = function(c_bra, c_ket, s)
    {
        var adjustment = s.length - (c_ket - c_bra);
        this.current = this.current.slice(0, c_bra) + s + this.current.slice(c_ket);
        this.limit += adjustment;
        if (this.cursor >= c_ket) this.cursor += adjustment;
        else if (this.cursor > c_bra) this.cursor = c_bra;
        return adjustment;
    };

    this.slice_check = function()
    {
        if (this.bra < 0 ||
            this.bra > this.ket ||
            this.ket > this.limit ||
            this.limit > this.current.length)
        {
            return false;
        }
        return true;
    };

    this.slice_from = function(s)
    {
        var result = false;
        if (this.slice_check())
        {
            this.replace_s(this.bra, this.ket, s);
            result = true;
        }
        return result;
    };

    this.slice_del = function()
    {
        return this.slice_from("");
    };

    this.insert = function(c_bra, c_ket, s)
    {
        var adjustment = this.replace_s(c_bra, c_ket, s);
        if (c_bra <= this.bra) this.bra += adjustment;
        if (c_bra <= this.ket) this.ket += adjustment;
    };

    this.slice_to = function()
    {
        var result = '';
        if (this.slice_check())
        {
            result = this.current.slice(this.bra, this.ket);
        }
        return result;
    };

    this.assign_to = function()
    {
        return this.current.slice(0, this.limit);
    };
};

// Generated by Snowball 2.1.0 - https://snowballstem.org/

/**@constructor*/
SpanishStemmer = function() {
    var base = new BaseStemmer();
    /** @const */ var a_0 = [
        ["", -1, 6],
        ["\u00E1", 0, 1],
        ["\u00E9", 0, 2],
        ["\u00ED", 0, 3],
        ["\u00F3", 0, 4],
        ["\u00FA", 0, 5]
    ];

    /** @const */ var a_1 = [
        ["la", -1, -1],
        ["sela", 0, -1],
        ["le", -1, -1],
        ["me", -1, -1],
        ["se", -1, -1],
        ["lo", -1, -1],
        ["selo", 5, -1],
        ["las", -1, -1],
        ["selas", 7, -1],
        ["les", -1, -1],
        ["los", -1, -1],
        ["selos", 10, -1],
        ["nos", -1, -1]
    ];

    /** @const */ var a_2 = [
        ["ando", -1, 6],
        ["iendo", -1, 6],
        ["yendo", -1, 7],
        ["\u00E1ndo", -1, 2],
        ["i\u00E9ndo", -1, 1],
        ["ar", -1, 6],
        ["er", -1, 6],
        ["ir", -1, 6],
        ["\u00E1r", -1, 3],
        ["\u00E9r", -1, 4],
        ["\u00EDr", -1, 5]
    ];

    /** @const */ var a_3 = [
        ["ic", -1, -1],
        ["ad", -1, -1],
        ["os", -1, -1],
        ["iv", -1, 1]
    ];

    /** @const */ var a_4 = [
        ["able", -1, 1],
        ["ible", -1, 1],
        ["ante", -1, 1]
    ];

    /** @const */ var a_5 = [
        ["ic", -1, 1],
        ["abil", -1, 1],
        ["iv", -1, 1]
    ];

    /** @const */ var a_6 = [
        ["ica", -1, 1],
        ["ancia", -1, 2],
        ["encia", -1, 5],
        ["adora", -1, 2],
        ["osa", -1, 1],
        ["ista", -1, 1],
        ["iva", -1, 9],
        ["anza", -1, 1],
        ["log\u00EDa", -1, 3],
        ["idad", -1, 8],
        ["able", -1, 1],
        ["ible", -1, 1],
        ["ante", -1, 2],
        ["mente", -1, 7],
        ["amente", 13, 6],
        ["aci\u00F3n", -1, 2],
        ["uci\u00F3n", -1, 4],
        ["ico", -1, 1],
        ["ismo", -1, 1],
        ["oso", -1, 1],
        ["amiento", -1, 1],
        ["imiento", -1, 1],
        ["ivo", -1, 9],
        ["ador", -1, 2],
        ["icas", -1, 1],
        ["ancias", -1, 2],
        ["encias", -1, 5],
        ["adoras", -1, 2],
        ["osas", -1, 1],
        ["istas", -1, 1],
        ["ivas", -1, 9],
        ["anzas", -1, 1],
        ["log\u00EDas", -1, 3],
        ["idades", -1, 8],
        ["ables", -1, 1],
        ["ibles", -1, 1],
        ["aciones", -1, 2],
        ["uciones", -1, 4],
        ["adores", -1, 2],
        ["antes", -1, 2],
        ["icos", -1, 1],
        ["ismos", -1, 1],
        ["osos", -1, 1],
        ["amientos", -1, 1],
        ["imientos", -1, 1],
        ["ivos", -1, 9]
    ];

    /** @const */ var a_7 = [
        ["ya", -1, 1],
        ["ye", -1, 1],
        ["yan", -1, 1],
        ["yen", -1, 1],
        ["yeron", -1, 1],
        ["yendo", -1, 1],
        ["yo", -1, 1],
        ["yas", -1, 1],
        ["yes", -1, 1],
        ["yais", -1, 1],
        ["yamos", -1, 1],
        ["y\u00F3", -1, 1]
    ];

    /** @const */ var a_8 = [
        ["aba", -1, 2],
        ["ada", -1, 2],
        ["ida", -1, 2],
        ["ara", -1, 2],
        ["iera", -1, 2],
        ["\u00EDa", -1, 2],
        ["ar\u00EDa", 5, 2],
        ["er\u00EDa", 5, 2],
        ["ir\u00EDa", 5, 2],
        ["ad", -1, 2],
        ["ed", -1, 2],
        ["id", -1, 2],
        ["ase", -1, 2],
        ["iese", -1, 2],
        ["aste", -1, 2],
        ["iste", -1, 2],
        ["an", -1, 2],
        ["aban", 16, 2],
        ["aran", 16, 2],
        ["ieran", 16, 2],
        ["\u00EDan", 16, 2],
        ["ar\u00EDan", 20, 2],
        ["er\u00EDan", 20, 2],
        ["ir\u00EDan", 20, 2],
        ["en", -1, 1],
        ["asen", 24, 2],
        ["iesen", 24, 2],
        ["aron", -1, 2],
        ["ieron", -1, 2],
        ["ar\u00E1n", -1, 2],
        ["er\u00E1n", -1, 2],
        ["ir\u00E1n", -1, 2],
        ["ado", -1, 2],
        ["ido", -1, 2],
        ["ando", -1, 2],
        ["iendo", -1, 2],
        ["ar", -1, 2],
        ["er", -1, 2],
        ["ir", -1, 2],
        ["as", -1, 2],
        ["abas", 39, 2],
        ["adas", 39, 2],
        ["idas", 39, 2],
        ["aras", 39, 2],
        ["ieras", 39, 2],
        ["\u00EDas", 39, 2],
        ["ar\u00EDas", 45, 2],
        ["er\u00EDas", 45, 2],
        ["ir\u00EDas", 45, 2],
        ["es", -1, 1],
        ["ases", 49, 2],
        ["ieses", 49, 2],
        ["abais", -1, 2],
        ["arais", -1, 2],
        ["ierais", -1, 2],
        ["\u00EDais", -1, 2],
        ["ar\u00EDais", 55, 2],
        ["er\u00EDais", 55, 2],
        ["ir\u00EDais", 55, 2],
        ["aseis", -1, 2],
        ["ieseis", -1, 2],
        ["asteis", -1, 2],
        ["isteis", -1, 2],
        ["\u00E1is", -1, 2],
        ["\u00E9is", -1, 1],
        ["ar\u00E9is", 64, 2],
        ["er\u00E9is", 64, 2],
        ["ir\u00E9is", 64, 2],
        ["ados", -1, 2],
        ["idos", -1, 2],
        ["amos", -1, 2],
        ["\u00E1bamos", 70, 2],
        ["\u00E1ramos", 70, 2],
        ["i\u00E9ramos", 70, 2],
        ["\u00EDamos", 70, 2],
        ["ar\u00EDamos", 74, 2],
        ["er\u00EDamos", 74, 2],
        ["ir\u00EDamos", 74, 2],
        ["emos", -1, 1],
        ["aremos", 78, 2],
        ["eremos", 78, 2],
        ["iremos", 78, 2],
        ["\u00E1semos", 78, 2],
        ["i\u00E9semos", 78, 2],
        ["imos", -1, 2],
        ["ar\u00E1s", -1, 2],
        ["er\u00E1s", -1, 2],
        ["ir\u00E1s", -1, 2],
        ["\u00EDs", -1, 2],
        ["ar\u00E1", -1, 2],
        ["er\u00E1", -1, 2],
        ["ir\u00E1", -1, 2],
        ["ar\u00E9", -1, 2],
        ["er\u00E9", -1, 2],
        ["ir\u00E9", -1, 2],
        ["i\u00F3", -1, 2]
    ];

    /** @const */ var a_9 = [
        ["a", -1, 1],
        ["e", -1, 2],
        ["o", -1, 1],
        ["os", -1, 1],
        ["\u00E1", -1, 1],
        ["\u00E9", -1, 2],
        ["\u00ED", -1, 1],
        ["\u00F3", -1, 1]
    ];

    /** @const */ var /** Array<int> */ g_v = [17, 65, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 17, 4, 10];

    var /** number */ I_p2 = 0;
    var /** number */ I_p1 = 0;
    var /** number */ I_pV = 0;


    /** @return {boolean} */
    function r_mark_regions() {
        I_pV = base.limit;
        I_p1 = base.limit;
        I_p2 = base.limit;
        var /** number */ v_1 = base.cursor;
        lab0: {
            lab1: {
                var /** number */ v_2 = base.cursor;
                lab2: {
                    if (!(base.in_grouping(g_v, 97, 252)))
                    {
                        break lab2;
                    }
                    lab3: {
                        var /** number */ v_3 = base.cursor;
                        lab4: {
                            if (!(base.out_grouping(g_v, 97, 252)))
                            {
                                break lab4;
                            }
                            golab5: while(true)
                            {
                                lab6: {
                                    if (!(base.in_grouping(g_v, 97, 252)))
                                    {
                                        break lab6;
                                    }
                                    break golab5;
                                }
                                if (base.cursor >= base.limit)
                                {
                                    break lab4;
                                }
                                base.cursor++;
                            }
                            break lab3;
                        }
                        base.cursor = v_3;
                        if (!(base.in_grouping(g_v, 97, 252)))
                        {
                            break lab2;
                        }
                        golab7: while(true)
                        {
                            lab8: {
                                if (!(base.out_grouping(g_v, 97, 252)))
                                {
                                    break lab8;
                                }
                                break golab7;
                            }
                            if (base.cursor >= base.limit)
                            {
                                break lab2;
                            }
                            base.cursor++;
                        }
                    }
                    break lab1;
                }
                base.cursor = v_2;
                if (!(base.out_grouping(g_v, 97, 252)))
                {
                    break lab0;
                }
                lab9: {
                    var /** number */ v_6 = base.cursor;
                    lab10: {
                        if (!(base.out_grouping(g_v, 97, 252)))
                        {
                            break lab10;
                        }
                        golab11: while(true)
                        {
                            lab12: {
                                if (!(base.in_grouping(g_v, 97, 252)))
                                {
                                    break lab12;
                                }
                                break golab11;
                            }
                            if (base.cursor >= base.limit)
                            {
                                break lab10;
                            }
                            base.cursor++;
                        }
                        break lab9;
                    }
                    base.cursor = v_6;
                    if (!(base.in_grouping(g_v, 97, 252)))
                    {
                        break lab0;
                    }
                    if (base.cursor >= base.limit)
                    {
                        break lab0;
                    }
                    base.cursor++;
                }
            }
            I_pV = base.cursor;
        }
        base.cursor = v_1;
        var /** number */ v_8 = base.cursor;
        lab13: {
            golab14: while(true)
            {
                lab15: {
                    if (!(base.in_grouping(g_v, 97, 252)))
                    {
                        break lab15;
                    }
                    break golab14;
                }
                if (base.cursor >= base.limit)
                {
                    break lab13;
                }
                base.cursor++;
            }
            golab16: while(true)
            {
                lab17: {
                    if (!(base.out_grouping(g_v, 97, 252)))
                    {
                        break lab17;
                    }
                    break golab16;
                }
                if (base.cursor >= base.limit)
                {
                    break lab13;
                }
                base.cursor++;
            }
            I_p1 = base.cursor;
            golab18: while(true)
            {
                lab19: {
                    if (!(base.in_grouping(g_v, 97, 252)))
                    {
                        break lab19;
                    }
                    break golab18;
                }
                if (base.cursor >= base.limit)
                {
                    break lab13;
                }
                base.cursor++;
            }
            golab20: while(true)
            {
                lab21: {
                    if (!(base.out_grouping(g_v, 97, 252)))
                    {
                        break lab21;
                    }
                    break golab20;
                }
                if (base.cursor >= base.limit)
                {
                    break lab13;
                }
                base.cursor++;
            }
            I_p2 = base.cursor;
        }
        base.cursor = v_8;
        return true;
    };

    /** @return {boolean} */
    function r_postlude() {
        var /** number */ among_var;
        while(true)
        {
            var /** number */ v_1 = base.cursor;
            lab0: {
                base.bra = base.cursor;
                among_var = base.find_among(a_0);
                if (among_var == 0)
                {
                    break lab0;
                }
                base.ket = base.cursor;
                switch (among_var) {
                    case 1:
                        if (!base.slice_from("a"))
                        {
                            return false;
                        }
                        break;
                    case 2:
                        if (!base.slice_from("e"))
                        {
                            return false;
                        }
                        break;
                    case 3:
                        if (!base.slice_from("i"))
                        {
                            return false;
                        }
                        break;
                    case 4:
                        if (!base.slice_from("o"))
                        {
                            return false;
                        }
                        break;
                    case 5:
                        if (!base.slice_from("u"))
                        {
                            return false;
                        }
                        break;
                    case 6:
                        if (base.cursor >= base.limit)
                        {
                            break lab0;
                        }
                        base.cursor++;
                        break;
                }
                continue;
            }
            base.cursor = v_1;
            break;
        }
        return true;
    };

    /** @return {boolean} */
    function r_RV() {
        if (!(I_pV <= base.cursor))
        {
            return false;
        }
        return true;
    };

    /** @return {boolean} */
    function r_R1() {
        if (!(I_p1 <= base.cursor))
        {
            return false;
        }
        return true;
    };

    /** @return {boolean} */
    function r_R2() {
        if (!(I_p2 <= base.cursor))
        {
            return false;
        }
        return true;
    };

    /** @return {boolean} */
    function r_attached_pronoun() {
        var /** number */ among_var;
        base.ket = base.cursor;
        if (base.find_among_b(a_1) == 0)
        {
            return false;
        }
        base.bra = base.cursor;
        among_var = base.find_among_b(a_2);
        if (among_var == 0)
        {
            return false;
        }
        if (!r_RV())
        {
            return false;
        }
        switch (among_var) {
            case 1:
                base.bra = base.cursor;
                if (!base.slice_from("iendo"))
                {
                    return false;
                }
                break;
            case 2:
                base.bra = base.cursor;
                if (!base.slice_from("ando"))
                {
                    return false;
                }
                break;
            case 3:
                base.bra = base.cursor;
                if (!base.slice_from("ar"))
                {
                    return false;
                }
                break;
            case 4:
                base.bra = base.cursor;
                if (!base.slice_from("er"))
                {
                    return false;
                }
                break;
            case 5:
                base.bra = base.cursor;
                if (!base.slice_from("ir"))
                {
                    return false;
                }
                break;
            case 6:
                if (!base.slice_del())
                {
                    return false;
                }
                break;
            case 7:
                if (!(base.eq_s_b("u")))
                {
                    return false;
                }
                if (!base.slice_del())
                {
                    return false;
                }
                break;
        }
        return true;
    };

    /** @return {boolean} */
    function r_standard_suffix() {
        var /** number */ among_var;
        base.ket = base.cursor;
        among_var = base.find_among_b(a_6);
        if (among_var == 0)
        {
            return false;
        }
        base.bra = base.cursor;
        switch (among_var) {
            case 1:
                if (!r_R2())
                {
                    return false;
                }
                if (!base.slice_del())
                {
                    return false;
                }
                break;
            case 2:
                if (!r_R2())
                {
                    return false;
                }
                if (!base.slice_del())
                {
                    return false;
                }
                var /** number */ v_1 = base.limit - base.cursor;
                lab0: {
                    base.ket = base.cursor;
                    if (!(base.eq_s_b("ic")))
                    {
                        base.cursor = base.limit - v_1;
                        break lab0;
                    }
                    base.bra = base.cursor;
                    if (!r_R2())
                    {
                        base.cursor = base.limit - v_1;
                        break lab0;
                    }
                    if (!base.slice_del())
                    {
                        return false;
                    }
                }
                break;
            case 3:
                if (!r_R2())
                {
                    return false;
                }
                if (!base.slice_from("log"))
                {
                    return false;
                }
                break;
            case 4:
                if (!r_R2())
                {
                    return false;
                }
                if (!base.slice_from("u"))
                {
                    return false;
                }
                break;
            case 5:
                if (!r_R2())
                {
                    return false;
                }
                if (!base.slice_from("ente"))
                {
                    return false;
                }
                break;
            case 6:
                if (!r_R1())
                {
                    return false;
                }
                if (!base.slice_del())
                {
                    return false;
                }
                var /** number */ v_2 = base.limit - base.cursor;
                lab1: {
                    base.ket = base.cursor;
                    among_var = base.find_among_b(a_3);
                    if (among_var == 0)
                    {
                        base.cursor = base.limit - v_2;
                        break lab1;
                    }
                    base.bra = base.cursor;
                    if (!r_R2())
                    {
                        base.cursor = base.limit - v_2;
                        break lab1;
                    }
                    if (!base.slice_del())
                    {
                        return false;
                    }
                    switch (among_var) {
                        case 1:
                            base.ket = base.cursor;
                            if (!(base.eq_s_b("at")))
                            {
                                base.cursor = base.limit - v_2;
                                break lab1;
                            }
                            base.bra = base.cursor;
                            if (!r_R2())
                            {
                                base.cursor = base.limit - v_2;
                                break lab1;
                            }
                            if (!base.slice_del())
                            {
                                return false;
                            }
                            break;
                    }
                }
                break;
            case 7:
                if (!r_R2())
                {
                    return false;
                }
                if (!base.slice_del())
                {
                    return false;
                }
                var /** number */ v_3 = base.limit - base.cursor;
                lab2: {
                    base.ket = base.cursor;
                    if (base.find_among_b(a_4) == 0)
                    {
                        base.cursor = base.limit - v_3;
                        break lab2;
                    }
                    base.bra = base.cursor;
                    if (!r_R2())
                    {
                        base.cursor = base.limit - v_3;
                        break lab2;
                    }
                    if (!base.slice_del())
                    {
                        return false;
                    }
                }
                break;
            case 8:
                if (!r_R2())
                {
                    return false;
                }
                if (!base.slice_del())
                {
                    return false;
                }
                var /** number */ v_4 = base.limit - base.cursor;
                lab3: {
                    base.ket = base.cursor;
                    if (base.find_among_b(a_5) == 0)
                    {
                        base.cursor = base.limit - v_4;
                        break lab3;
                    }
                    base.bra = base.cursor;
                    if (!r_R2())
                    {
                        base.cursor = base.limit - v_4;
                        break lab3;
                    }
                    if (!base.slice_del())
                    {
                        return false;
                    }
                }
                break;
            case 9:
                if (!r_R2())
                {
                    return false;
                }
                if (!base.slice_del())
                {
                    return false;
                }
                var /** number */ v_5 = base.limit - base.cursor;
                lab4: {
                    base.ket = base.cursor;
                    if (!(base.eq_s_b("at")))
                    {
                        base.cursor = base.limit - v_5;
                        break lab4;
                    }
                    base.bra = base.cursor;
                    if (!r_R2())
                    {
                        base.cursor = base.limit - v_5;
                        break lab4;
                    }
                    if (!base.slice_del())
                    {
                        return false;
                    }
                }
                break;
        }
        return true;
    };

    /** @return {boolean} */
    function r_y_verb_suffix() {
        if (base.cursor < I_pV)
        {
            return false;
        }
        var /** number */ v_2 = base.limit_backward;
        base.limit_backward = I_pV;
        base.ket = base.cursor;
        if (base.find_among_b(a_7) == 0)
        {
            base.limit_backward = v_2;
            return false;
        }
        base.bra = base.cursor;
        base.limit_backward = v_2;
        if (!(base.eq_s_b("u")))
        {
            return false;
        }
        if (!base.slice_del())
        {
            return false;
        }
        return true;
    };

    /** @return {boolean} */
    function r_verb_suffix() {
        var /** number */ among_var;
        if (base.cursor < I_pV)
        {
            return false;
        }
        var /** number */ v_2 = base.limit_backward;
        base.limit_backward = I_pV;
        base.ket = base.cursor;
        among_var = base.find_among_b(a_8);
        if (among_var == 0)
        {
            base.limit_backward = v_2;
            return false;
        }
        base.bra = base.cursor;
        base.limit_backward = v_2;
        switch (among_var) {
            case 1:
                var /** number */ v_3 = base.limit - base.cursor;
                lab0: {
                    if (!(base.eq_s_b("u")))
                    {
                        base.cursor = base.limit - v_3;
                        break lab0;
                    }
                    var /** number */ v_4 = base.limit - base.cursor;
                    if (!(base.eq_s_b("g")))
                    {
                        base.cursor = base.limit - v_3;
                        break lab0;
                    }
                    base.cursor = base.limit - v_4;
                }
                base.bra = base.cursor;
                if (!base.slice_del())
                {
                    return false;
                }
                break;
            case 2:
                if (!base.slice_del())
                {
                    return false;
                }
                break;
        }
        return true;
    };

    /** @return {boolean} */
    function r_residual_suffix() {
        var /** number */ among_var;
        base.ket = base.cursor;
        among_var = base.find_among_b(a_9);
        if (among_var == 0)
        {
            return false;
        }
        base.bra = base.cursor;
        switch (among_var) {
            case 1:
                if (!r_RV())
                {
                    return false;
                }
                if (!base.slice_del())
                {
                    return false;
                }
                break;
            case 2:
                if (!r_RV())
                {
                    return false;
                }
                if (!base.slice_del())
                {
                    return false;
                }
                var /** number */ v_1 = base.limit - base.cursor;
                lab0: {
                    base.ket = base.cursor;
                    if (!(base.eq_s_b("u")))
                    {
                        base.cursor = base.limit - v_1;
                        break lab0;
                    }
                    base.bra = base.cursor;
                    var /** number */ v_2 = base.limit - base.cursor;
                    if (!(base.eq_s_b("g")))
                    {
                        base.cursor = base.limit - v_1;
                        break lab0;
                    }
                    base.cursor = base.limit - v_2;
                    if (!r_RV())
                    {
                        base.cursor = base.limit - v_1;
                        break lab0;
                    }
                    if (!base.slice_del())
                    {
                        return false;
                    }
                }
                break;
        }
        return true;
    };

    this.stem = /** @return {boolean} */ function() {
        r_mark_regions();
        base.limit_backward = base.cursor; base.cursor = base.limit;
        var /** number */ v_2 = base.limit - base.cursor;
        r_attached_pronoun();
        base.cursor = base.limit - v_2;
        var /** number */ v_3 = base.limit - base.cursor;
        lab0: {
            lab1: {
                var /** number */ v_4 = base.limit - base.cursor;
                lab2: {
                    if (!r_standard_suffix())
                    {
                        break lab2;
                    }
                    break lab1;
                }
                base.cursor = base.limit - v_4;
                lab3: {
                    if (!r_y_verb_suffix())
                    {
                        break lab3;
                    }
                    break lab1;
                }
                base.cursor = base.limit - v_4;
                if (!r_verb_suffix())
                {
                    break lab0;
                }
            }
        }
        base.cursor = base.limit - v_3;
        var /** number */ v_5 = base.limit - base.cursor;
        r_residual_suffix();
        base.cursor = base.limit - v_5;
        base.cursor = base.limit_backward;
        var /** number */ v_6 = base.cursor;
        r_postlude();
        base.cursor = v_6;
        return true;
    };

    /**@return{string}*/
    this['stemWord'] = function(/**string*/word) {
        base.setCurrent(word);
        this.stem();
        return base.getCurrent();
    };
};

Stemmer = SpanishStemmer;
