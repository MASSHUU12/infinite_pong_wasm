const ctx = document.getElementById("game").getContext("2d");

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

          const r = ((colorValue >> (8 * 0)) & 0xff)
            .toString(16)
            .padStart(2, 0);
          const g = ((colorValue >> (8 * 1)) & 0xff)
            .toString(16)
            .padStart(2, 0);
          const b = ((colorValue >> (8 * 2)) & 0xff)
            .toString(16)
            .padStart(2, 0);
          const a = ((colorValue >> (8 * 3)) & 0xff)
            .toString(16)
            .padStart(2, 0);

          ctx.fillStyle = "#" + r + g + b + a;
          ctx.fillRect(0, 0, ctx.canvas.width, ctx.canvas.height);
        },
      },
    },
  );

  console.log(instance);
  console.log(ctx);
  instance.exports.main();
}
init();
