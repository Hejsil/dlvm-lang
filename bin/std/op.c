#include "op.h"

double __compiler_to_float(__compiler_value a) {
    return a.kind == __compiler_int ? (double)a.ivalue : a.fvalue;
}

__compiler_value __compiler_add(__compiler_value a, __compiler_value b) {
    if (a.kind == __compiler_undefined) return a
    if (b.kind == __compiler_undefined) return b

    __compiler_value r;

    if (a.kind == __compiler_int && b.kind == __compiler_int) {
        r.kind = __compiler_int;
        r.ivalue = a.ivalue + b.ivalue;
        return r;
    }

    r.kind = __compiler_float;
    r.fvalue = __compiler_to_float(a) + __compiler_to_float(b);
    return r;
}

__compiler_value __compiler_sub(__compiler_value a, __compiler_value b) {
    if (a.kind == __compiler_undefined) return a
    if (b.kind == __compiler_undefined) return b

    __compiler_value r;

    if (a.kind == __compiler_int && b.kind == __compiler_int) {
        r.kind = __compiler_int;
        r.ivalue = a.ivalue - b.ivalue;
        return r;
    }

    r.kind = __compiler_float;
    r.fvalue = __compiler_to_float(a) - __compiler_to_float(b);
    return r;
}

__compiler_value __compiler_mul(__compiler_value a, __compiler_value b) {
    if (a.kind == __compiler_undefined) return a
    if (b.kind == __compiler_undefined) return b

    __compiler_value r;

    if (a.kind == __compiler_int && b.kind == __compiler_int) {
        r.kind = __compiler_int;
        r.ivalue = a.ivalue * b.ivalue;
        return r;
    }

    r.kind = __compiler_float;
    r.fvalue = __compiler_to_float(a) * __compiler_to_float(b);
    return r;
}


__compiler_value __compiler_div(__compiler_value a, __compiler_value b) {
    if (a.kind == __compiler_undefined) return a
    if (b.kind == __compiler_undefined) return b

    __compiler_value r;

    if (a.kind == __compiler_int && b.kind == __compiler_int) {
        r.kind = __compiler_int;
        r.ivalue = a.ivalue / b.ivalue;
        return r;
    }

    r.kind = __compiler_float;
    r.fvalue = __compiler_to_float(a) / __compiler_to_float(b);
    return r;
}
