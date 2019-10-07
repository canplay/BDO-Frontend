export default {
  nums: [
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "0",
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    "G",
    "H",
    "I",
    "J",
    "K",
    "L",
    "M",
    "N",
    "O",
    "P",
    "Q",
    "R",
    "S",
    "T",
    "U",
    "V",
    "W",
    "X",
    "Y",
    "Z",
    "a",
    "b",
    "c",
    "d",
    "e",
    "f",
    "g",
    "h",
    "i",
    "j",
    "k",
    "l",
    "m",
    "n",
    "o",
    "p",
    "q",
    "r",
    "s",
    "t",
    "u",
    "v",
    "w",
    "x",
    "y",
    "z"
  ],

  drawCode(canvas, img) {
    let context = canvas.getContext("2d");
    context.fillStyle = "white";
    context.fillRect(0, 0, canvas.width, canvas.height);
    context.fillStyle = "black";
    context.font = "30px Arial";

    let rand = new Array();
    let x = new Array();
    let y = new Array();
    for (let i = 0; i < 4; i++) {
      rand.push(rand[i]);
      rand[i] = this.nums[Math.floor(Math.random() * this.nums.length)];
      x[i] = i * 12 + 5;
      y[i] = Math.random() * 20 + 32;
      context.fillText(rand[i], x[i], y[i]);
    }

    let str = rand.join("");

    for (let i = 0; i < 3; i++) {
      this.drawline(canvas, context);
    }

    for (let i = 0; i < 30; i++) {
      this.drawDot(canvas, context);
    }

    img.src = canvas.toDataURL("image/png");

    return str;
  },

  drawline(canvas, context) {
    context.moveTo(
      Math.floor(Math.random() * canvas.width),
      Math.floor(Math.random() * canvas.height)
    );
    context.lineTo(
      Math.floor(Math.random() * canvas.width),
      Math.floor(Math.random() * canvas.height)
    );
    context.lineWidth = 0.5;
    context.strokeStyle = "rgba(50, 50, 50, 0.3)";
    context.stroke();
  },

  drawDot(canvas, context) {
    var px = Math.floor(Math.random() * canvas.width);
    var py = Math.floor(Math.random() * canvas.height);
    context.moveTo(px, py);
    context.lineTo(px + 1, py + 1);
    context.lineWidth = 0.2;
    context.stroke();
  }
};
