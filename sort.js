function sieve(n) {
    var a = new Array(n);
    for (var i = 2; i < n; i++) {
        a[i] = true;
    }
    for (var i = 2; i < Math.sqrt(n); i++) {
        for (var j = i * i; j < n; j += i) {
            if (a[j]) {
                a[j] = false;
            }
        }
    }
    var b = new Array();
    for (var i = 0; i < n; i++) {
        if (a[i]) {
            b.push(i);
        }
    }
    return b;
}
function quicksort(a, lo, hi) {
    if (lo < hi) {
        var p = partition(a, lo, hi);
        quicksort(a, p + 1, hi);
        quicksort(a, lo, p - 1);
    }
}
function partition(a, lo, hi) {
    var pivot = a[hi];
    var i = lo - 1;
    for (var j = lo; j < hi; j++) {
        if (pivot > a[j]) {
            i++;
            var tmp_1 = a[i];
            a[i] = a[j];
            a[j] = tmp_1;
        }
    }
    var tmp = a[i + 1];
    a[i + 1] = a[hi];
    a[hi] = tmp;
    return i + 1;
}
function main() {
    console.log("Hello World");
    var a = [24, 13, 34221, 34, 43, 1, 1, 4346, 36, 4, 234, 7, 8456, 24, 24];
    console.log(a);
    quicksort(a, 0, a.length - 1);
    console.log(a);
    var b = sieve(232423);
    console.log(b);
    console.log(b[b.length - 1]);
}
main();
