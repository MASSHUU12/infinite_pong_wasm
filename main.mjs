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
      },
    },
  );

  console.log(instance);
  console.log(ctx);
  instance.exports.main();
}
init();
