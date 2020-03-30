import { writeFileSync } from "fs";
import cheerio from "cheerio";
import fetch from "node-fetch";

const sizes = [6, 8, 10, 12, 14];
const levels = [1, 2, 3, 4];
let i = 0;

const get = (size: number, level: number, nr: number): any => {
  return fetch(
    `http://www.binarypuzzle.com/puzzles.php?size=${size}&level=${level}&nr=${nr}`
  )
    .then(resp => resp.text())
    .then(html => cheerio.load(html))
    .then($ =>
      Array.from($(`p[id*="celpar"]`)).map(ele => {
        const [, x, y] = ele.attribs["id"].split("_");
        const val = ele.children[0].data?.trim();
        return { x, y, val };
      })
    )
    .then(triple =>
      triple
        .filter(ele => ele.val)
        .map(({ x, y, val }) => `${x} ${y} ${val}`)
        .join("\n")
    )
    .then(data => {
      const meta = `${size}\n`;
      writeFileSync(`./out/s${size}_l${level}_${nr}.txt`, meta + data);
    })
    .catch(err => get(size, level, nr));
};

for (const size of sizes) {
  for (const level of levels) {
    for (let nr = 1; nr <= 100; nr++) {
      setTimeout(() => {
        get(size, level, nr);
      }, i++ * 500);
    }
  }
}
