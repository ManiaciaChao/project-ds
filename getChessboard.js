const cheerio = require("cheerio");
const fetch = require("node-fetch");

const sizes = [6, 8, 10, 12, 14];
const levels = [1, 2, 3, 4];

const size = 6;
const level = 1;
const nr = 1;
fetch(
  `http://www.binarypuzzle.com/puzzles.php?size=${size}&level=${level}&nr=${nr}`
)
  .then(resp => resp.text())
  .then(html => cheerio.load(html))
  .then($ => {
    console.log(
      Array.from($(`p[id*="celpar"]`)).map(ele => ({
        id: ele.attr("id"),
        text: ele.text()
      }))
    );
  });

// for (const size of sizes) {
//   for (const level of levels) {
//     for (let nr = 1; nr <= 100; nr++) {}
//   }
// }

// Array.from(document.querySelectorAll(`p[id*="celpar"]`))
//   .map(({ id, textContent }) => ({ id, val: textContent.trim() }))
//   .filter(obj => obj.val)
//   .map(({ id, val }) => {
//     const [, ...prefix] = id.split("_");
//     return parseInt(prefix.join("")) * (val === "1" ? 1 : -1);
//   })
//   .join(" ") + " 0";
