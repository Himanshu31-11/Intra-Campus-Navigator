/* ---------- Backend address ---------- */
// If the page is opened through the C++ server, use the same address.
// If it is opened directly from the file system, point to the server.
const API = location.protocol === "file:" ? "http://localhost:8080" : "";

/* Map positions (x, y) used only for drawing */
const pos = {
Gate1:[60,260],Entry:[170,260],MacLab:[170,160],ERPCell:[170,70],Library:[300,50],GirlsWashroom1:[290,120],
Hall1:[420,120],OfficeRoom1:[540,120],Lab2:[660,120],GirlsWashroom2:[780,80],Out:[830,230],
BoysWashroom1:[90,350],Hall2:[290,330],Office2:[420,330],Lab1:[660,330],BoysWashroom2:[760,330],
LiftStair:[480,230],Class1:[690,190],Class2:[740,265],Class3:[720,420],Class4:[620,450],Class5:[560,400],
SportsGround:[130,480],Gate2:[260,505],SeatingArea:[400,200],Canteen:[560,250]
};

/* ---------- Page elements ---------- */
const $ = id => document.getElementById(id);
const startSel = $("start"), destSel = $("dest"), result = $("result"), svg = $("map");
const NS = "http://www.w3.org/2000/svg";

let names = [];          // filled from the backend
let edgeList = [];       // filled from the backend
const edgeEls = {}, nodeEls = {};
let clickStep = 0;

function el(tag, attrs, parent){
  const e = document.createElementNS(NS, tag);
  for (const k in attrs) e.setAttribute(k, attrs[k]);
  parent.appendChild(e);
  return e;
}

function costBetween(a, b){
  const e = edgeList.find(x => (x.from === a && x.to === b) || (x.from === b && x.to === a));
  return e ? e.cost : 0;
}

/* ---------- Draw the map from backend data ---------- */
function drawMap(){
  svg.innerHTML = "";

  edgeList.forEach(({from, to}) => {
    const [x1,y1] = pos[from], [x2,y2] = pos[to];
    edgeEls[from+"|"+to] = edgeEls[to+"|"+from] = el("line",{x1,y1,x2,y2,class:"edge"},svg);
  });

  edgeList.forEach(({from, to, cost}) => {
    const [x1,y1] = pos[from], [x2,y2] = pos[to];
    const t = el("text",{x:(x1+x2)/2, y:(y1+y2)/2-4, class:"ecost", "text-anchor":"middle"},svg);
    t.textContent = cost;
  });

  names.forEach(n => {
    const g = el("g",{class:"node", tabindex:0, role:"button", "aria-label":n},svg);
    el("circle",{cx:pos[n][0], cy:pos[n][1], r:11},g);
    const t = el("text",{x:pos[n][0], y:pos[n][1]+25},g);
    t.textContent = n;
    nodeEls[n] = g;

    const pick = () => {
      if (clickStep === 0){ startSel.value = n; clickStep = 1; paint(); }
      else { destSel.value = n; clickStep = 0; run(); }
    };
    g.addEventListener("click", pick);
    g.addEventListener("keydown", e => {
      if (e.key === "Enter" || e.key === " "){ e.preventDefault(); pick(); }
    });
  });
}

/* Highlight the route on the map */
function paint(pathNames = []){
  Object.values(edgeEls).forEach(e => e.classList.remove("on"));
  for (let i = 0; i < pathNames.length - 1; i++){
    const line = edgeEls[pathNames[i]+"|"+pathNames[i+1]];
    if (line) line.classList.add("on");
  }
  names.forEach(n => {
    const g = nodeEls[n];
    g.classList.toggle("on", pathNames.includes(n));
    g.classList.toggle("start", n === startSel.value);
    g.classList.toggle("end", n === destSel.value && pathNames.length > 0);
  });
}

/* ---------- Ask the C++ backend for the route ---------- */
async function run(){
  const start = startSel.value, dest = destSel.value;
  try {
    const res = await fetch(`${API}/api/route?start=${encodeURIComponent(start)}&dest=${encodeURIComponent(dest)}`);
    const data = await res.json();

    if (!res.ok){
      result.innerHTML = `<p class="err">${data.error}</p>`;
      paint();
      return;
    }

    const p = data.path;
    let steps = "";
    p.forEach((n,i) => {
      const cost = i > 0 ? costBetween(p[i-1], n) : 0;
      steps += `<li>${n}${i > 0 ? ` <small>(+${cost})</small>` : ""}</li>`;
    });
    result.innerHTML =
      `<div class="cost">Total cost: ${data.totalCost}</div>
       <div class="steps-count">${p.length-1} step${p.length-1 === 1 ? "" : "s"}</div>
       <ol>${steps}</ol>`;
    paint(p);
  } catch (err) {
    result.innerHTML = `<p class="err">Cannot reach the backend. Start the C++ server and open http://localhost:8080</p>`;
  }
}

/* ---------- Start up: load the graph from the backend ---------- */
async function init(){
  try {
    const res = await fetch(`${API}/api/graph`);
    const data = await res.json();
    names = data.locations;
    edgeList = data.edges;

    names.forEach(n => { startSel.add(new Option(n,n)); destSel.add(new Option(n,n)); });
    startSel.value = "Gate1";
    destSel.value = "Library";

    drawMap();
    run();
  } catch (err) {
    result.innerHTML = `<p class="err">Cannot reach the backend. Start the C++ server and open http://localhost:8080</p>`;
  }
}

/* Events */
$("go").addEventListener("click", run);
$("swap").addEventListener("click", () => {
  [startSel.value, destSel.value] = [destSel.value, startSel.value];
  run();
});
startSel.addEventListener("change", run);
destSel.addEventListener("change", run);

init();