export const idToPos = (id: number, order: number) => {
  if (id >= order * order) {
    return { x: 0, y: 0 };
  }
  let x = (id + 1) % order;
  let y = id / order + 1;
  if (x == 0) {
    x = order;
  }
  return { x, y };
};

export const posToId = (x: number, y: number, order: number) =>
  (x - 1) * order + y - 1;
