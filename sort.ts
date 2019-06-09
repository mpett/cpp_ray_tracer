function sieve(n:number):number[] {
    let a:boolean[] = new Array(n);

    for (let i:number = 2; i < n; i++) {
        a[i] = true;
    }

    for (let i:number = 2; i < Math.sqrt(n); i++) {
        for (let j:number = i*i; j < n; j += i) {
            if (a[j]) {
                a[j] = false;
            }
        }
    }

    let b:number[] = new Array();

    for (let i:number = 0; i < n; i++) {
        if (a[i]) {
            b.push(i);
        }
    }
    
    return b;
}

function quicksort(a:number[], lo:number, hi:number):void {
    if (lo < hi) {
        let p:number = partition(a, lo, hi);
        quicksort(a, p+1, hi);
        quicksort(a, lo, p-1);
    }
}

function partition(a:number[], lo:number, hi:number):number {
    let pivot:number = a[hi];
    let i:number = lo - 1;

    for (let j:number = lo; j < hi; j++) {
        if (pivot > a[j]) {
            i++;
            let tmp:number = a[i];
            a[i] = a[j];
            a[j] = tmp;
        }
    }

    let tmp:number = a[hi];
    a[hi] = a[i+1];
    a[i+1] = tmp;
    
    return i+1;
}

function main():void {
    console.log("Hello World");
    let a:number[] = [232,435,12,35,4,4,4,2,352,5313,34,6,7,34,23,41,21];
    console.log(a);
    quicksort(a, 0, a.length - 1);
    console.log(a);
    let b:number[] = sieve(4324223);
    console.log(b);
    console.log(b[b.length-1]);
}

main();