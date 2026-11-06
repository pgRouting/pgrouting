/*
 * This script contains the language-specific data used by searchtools.js,
 * namely the list of stopwords, stemmer, scorer and splitter.
 */

var stopwords = ["alla", "allt", "att", "av", "blev", "bli", "blir", "blivit", "de", "dem", "den", "denna", "deras", "dess", "dessa", "det", "detta", "dig", "din", "dina", "ditt", "du", "d\u00e4r", "d\u00e5", "efter", "ej", "eller", "en", "er", "era", "ert", "ett", "fr\u00e5n", "f\u00f6r", "ha", "hade", "han", "hans", "har", "henne", "hennes", "hon", "honom", "hur", "h\u00e4r", "i", "icke", "ingen", "inom", "inte", "jag", "ju", "kan", "kunde", "man", "med", "mellan", "men", "mig", "min", "mina", "mitt", "mot", "mycket", "ni", "nu", "n\u00e4r", "n\u00e5gon", "n\u00e5got", "n\u00e5gra", "och", "om", "oss", "p\u00e5", "samma", "sedan", "sig", "sin", "sina", "sitta", "sj\u00e4lv", "skulle", "som", "s\u00e5", "s\u00e5dan", "s\u00e5dana", "s\u00e5dant", "till", "under", "upp", "ut", "utan", "vad", "var", "vara", "varf\u00f6r", "varit", "varje", "vars", "vart", "vem", "vi", "vid", "vilka", "vilkas", "vilken", "vilket", "v\u00e5r", "v\u00e5ra", "v\u00e5rt", "\u00e4n", "\u00e4r", "\u00e5t", "\u00f6ver"];


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
SwedishStemmer = function() {
    var base = new BaseStemmer();
    /** @const */ var a_0 = [
        ["a", -1, 1],
        ["arna", 0, 1],
        ["erna", 0, 1],
        ["heterna", 2, 1],
        ["orna", 0, 1],
        ["ad", -1, 1],
        ["e", -1, 1],
        ["ade", 6, 1],
        ["ande", 6, 1],
        ["arne", 6, 1],
        ["are", 6, 1],
        ["aste", 6, 1],
        ["en", -1, 1],
        ["anden", 12, 1],
        ["aren", 12, 1],
        ["heten", 12, 1],
        ["ern", -1, 1],
        ["ar", -1, 1],
        ["er", -1, 1],
        ["heter", 18, 1],
        ["or", -1, 1],
        ["s", -1, 2],
        ["as", 21, 1],
        ["arnas", 22, 1],
        ["ernas", 22, 1],
        ["ornas", 22, 1],
        ["es", 21, 1],
        ["ades", 26, 1],
        ["andes", 26, 1],
        ["ens", 21, 1],
        ["arens", 29, 1],
        ["hetens", 29, 1],
        ["erns", 21, 1],
        ["at", -1, 1],
        ["andet", -1, 1],
        ["het", -1, 1],
        ["ast", -1, 1]
    ];

    /** @const */ var a_1 = [
        ["dd", -1, -1],
        ["gd", -1, -1],
        ["nn", -1, -1],
        ["dt", -1, -1],
        ["gt", -1, -1],
        ["kt", -1, -1],
        ["tt", -1, -1]
    ];

    /** @const */ var a_2 = [
        ["ig", -1, 1],
        ["lig", 0, 1],
        ["els", -1, 1],
        ["fullt", -1, 3],
        ["l\u00F6st", -1, 2]
    ];

    /** @const */ var /** Array<int> */ g_v = [17, 65, 16, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 24, 0, 32];

    /** @const */ var /** Array<int> */ g_s_ending = [119, 127, 149];

    var /** number */ I_x = 0;
    var /** number */ I_p1 = 0;


    /** @return {boolean} */
    function r_mark_regions() {
        I_p1 = base.limit;
        var /** number */ v_1 = base.cursor;
        {
            var /** number */ c1 = base.cursor + 3;
            if (c1 > base.limit)
            {
                return false;
            }
            base.cursor = c1;
        }
        I_x = base.cursor;
        base.cursor = v_1;
        golab0: while(true)
        {
            var /** number */ v_2 = base.cursor;
            lab1: {
                if (!(base.in_grouping(g_v, 97, 246)))
                {
                    break lab1;
                }
                base.cursor = v_2;
                break golab0;
            }
            base.cursor = v_2;
            if (base.cursor >= base.limit)
            {
                return false;
            }
            base.cursor++;
        }
        golab2: while(true)
        {
            lab3: {
                if (!(base.out_grouping(g_v, 97, 246)))
                {
                    break lab3;
                }
                break golab2;
            }
            if (base.cursor >= base.limit)
            {
                return false;
            }
            base.cursor++;
        }
        I_p1 = base.cursor;
        lab4: {
            if (!(I_p1 < I_x))
            {
                break lab4;
            }
            I_p1 = I_x;
        }
        return true;
    };

    /** @return {boolean} */
    function r_main_suffix() {
        var /** number */ among_var;
        if (base.cursor < I_p1)
        {
            return false;
        }
        var /** number */ v_2 = base.limit_backward;
        base.limit_backward = I_p1;
        base.ket = base.cursor;
        among_var = base.find_among_b(a_0);
        if (among_var == 0)
        {
            base.limit_backward = v_2;
            return false;
        }
        base.bra = base.cursor;
        base.limit_backward = v_2;
        switch (among_var) {
            case 1:
                if (!base.slice_del())
                {
                    return false;
                }
                break;
            case 2:
                if (!(base.in_grouping_b(g_s_ending, 98, 121)))
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
    function r_consonant_pair() {
        if (base.cursor < I_p1)
        {
            return false;
        }
        var /** number */ v_2 = base.limit_backward;
        base.limit_backward = I_p1;
        var /** number */ v_3 = base.limit - base.cursor;
        if (base.find_among_b(a_1) == 0)
        {
            base.limit_backward = v_2;
            return false;
        }
        base.cursor = base.limit - v_3;
        base.ket = base.cursor;
        if (base.cursor <= base.limit_backward)
        {
            base.limit_backward = v_2;
            return false;
        }
        base.cursor--;
        base.bra = base.cursor;
        if (!base.slice_del())
        {
            return false;
        }
        base.limit_backward = v_2;
        return true;
    };

    /** @return {boolean} */
    function r_other_suffix() {
        var /** number */ among_var;
        if (base.cursor < I_p1)
        {
            return false;
        }
        var /** number */ v_2 = base.limit_backward;
        base.limit_backward = I_p1;
        base.ket = base.cursor;
        among_var = base.find_among_b(a_2);
        if (among_var == 0)
        {
            base.limit_backward = v_2;
            return false;
        }
        base.bra = base.cursor;
        switch (among_var) {
            case 1:
                if (!base.slice_del())
                {
                    return false;
                }
                break;
            case 2:
                if (!base.slice_from("l\u00F6s"))
                {
                    return false;
                }
                break;
            case 3:
                if (!base.slice_from("full"))
                {
                    return false;
                }
                break;
        }
        base.limit_backward = v_2;
        return true;
    };

    this.stem = /** @return {boolean} */ function() {
        var /** number */ v_1 = base.cursor;
        r_mark_regions();
        base.cursor = v_1;
        base.limit_backward = base.cursor; base.cursor = base.limit;
        var /** number */ v_2 = base.limit - base.cursor;
        r_main_suffix();
        base.cursor = base.limit - v_2;
        var /** number */ v_3 = base.limit - base.cursor;
        r_consonant_pair();
        base.cursor = base.limit - v_3;
        var /** number */ v_4 = base.limit - base.cursor;
        r_other_suffix();
        base.cursor = base.limit - v_4;
        base.cursor = base.limit_backward;
        return true;
    };

    /**@return{string}*/
    this['stemWord'] = function(/**string*/word) {
        base.setCurrent(word);
        this.stem();
        return base.getCurrent();
    };
};

Stemmer = SwedishStemmer;
