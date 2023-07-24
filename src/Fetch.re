// async function logMovies() {
//   const response = await fetch("https://gist.githubusercontent.com/rusty-key/659db3f4566df459bd59c8a53dc9f71f/raw/4127f9550ef063121c564025f6d27dceeb279623/counties.json");
// console.log(response);
//   const movies = await response.json();
//   console.log(movies);
// }

// This is a simple binding to the Fetch API in order to make the component in this project functional
type response;

[@bs.val] external fetch: string => Js.Promise.t(response) = "fetch";
let fetch = url =>
  RJs.Promise.toIOLazy(() => url |> fetch) |> IO.mapError(ignore);

[@bs.send] external json: response => Js.Promise.t(Js.Json.t) = "json";
let json = response =>
  RJs.Promise.toIOLazy(() => response |> json) |> IO.mapError(ignore);
