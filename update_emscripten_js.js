const fs = require('fs');

const f = process.argv[2];
let js = fs.readFileSync(f).toString();

const i = js.indexOf('"env":asmLibraryArg,');
const i0 = js.indexOf('"intgemm":{');
const intgemmInit = `"intgemm":{
maddubs_signed_x8(a, b, sum0, sum1, sum2, sum3, sum4, sum5, sum6, sum7) {
    const sums = [sum0, sum1, sum2, sum3, sum4, sum5, sum6, sum7];
    for (let q = 0; q < 8; q++) {
        const sum = sums[q] >> 1;
        const bq = b + (q << 4);
        for (let i = 0; i < 8; i++) {
            const i2 = i << 1;
            HEAP16[sum + i] +=
                HEAP8[a + i2] * HEAP8[bq + i2] +
                HEAP8[a + i2 + 1] * HEAP8[bq + i2 + 1];
        }
    }
},
},`;
js = js.substring(0, i0 < 0 ? i : i0) + intgemmInit + js.substring(i);
fs.writeFileSync(f, js);