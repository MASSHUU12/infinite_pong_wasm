const ctx = document.getElementById("game").getContext("2d");

function parseColor(color) {
  const r = ((color >> (8 * 0)) & 0xff).toString(16).padStart(2, 0);
  const g = ((color >> (8 * 1)) & 0xff).toString(16).padStart(2, 0);
  const b = ((color >> (8 * 2)) & 0xff).toString(16).padStart(2, 0);
  const a = ((color >> (8 * 3)) & 0xff).toString(16).padStart(2, 0);

  return "#" + r + g + b + a;
}

async function init() {
  const { instance } = await WebAssembly.instantiateStreaming(
    fetch("./main.wasm"),
    {
      env: {
        set_canvas_size: function (w, h) {
          ctx.canvas.width = w;
          ctx.canvas.height = h;
        },
        clear_with_color: function (color) {
          const memory = instance.exports.memory;
          const colorValue = new Uint32Array(memory.buffer, color, 1)[0];

          ctx.fillStyle = parseColor(colorValue);
          ctx.fillRect(0, 0, ctx.canvas.width, ctx.canvas.height);
        },
        fill_rect: function (x, y, w, h, color) {
          const memory = instance.exports.memory;
          const colorValue = new Uint32Array(memory.buffer, color, 1)[0];

          ctx.fillStyle = parseColor(colorValue);
          ctx.fillRect(x, y, w, h);
        },
      },
    },
  );

  console.log(instance);
  console.log(ctx);
  instance.exports.main();
}
init();
