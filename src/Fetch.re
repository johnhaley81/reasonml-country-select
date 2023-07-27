// This is a simple binding to the Fetch API in order to make the component in this project functional
type response;

[@bs.val] external fetch: string => Js.Promise.t(response) = "fetch";
let fetch = url =>
  RJs.Promise.toIOLazy(() => url |> fetch) |> IO.mapError(ignore);

[@bs.send] external json: response => Js.Promise.t(Js.Json.t) = "json";
let json = response =>
  RJs.Promise.toIOLazy(() => response |> json) |> IO.mapError(ignore);
