let focusRef = domRef => domRef->ReactDOM.domElementToObj##focus();
let focusRef = domRef => domRef |> IO.map(Option.fold((), focusRef));
let blurRef = domRef => domRef->ReactDOM.domElementToObj##blur();
let blurRef = domRef => domRef |> IO.map(Option.fold((), blurRef));

[@bs.deriving accessors]
type key =
  | EscapeKey
  | EnterKey
  | ArrowUp
  | ArrowDown;

let fromString =
  fun
  | "Enter" => enterKey |> Option.pure
  | "Escape" => escapeKey |> Option.pure
  | "ArrowUp" => arrowUp |> Option.pure
  | "ArrowDown" => arrowDown |> Option.pure
  | _ => None;
let getKeyFromEvent = e =>
  e |> ReactEvent.Keyboard.key |> fromString |> Option.map(key => (key, e));
