[%%bs.raw "import \"/node_modules/flag-icons/css/flag-icons.min.css\""];

open ReactUtils;

type displayFormat =
  | Normal
  | Square;

let displayFormatToClassName =
  fun
  | Normal => "fi"
  | Square => "fi fis";

module CountryCode = {
  type t =
    | CountryCode(string);

  // This ensures that the string is a 2-letter string which will help it be more resilient at runtime.
  // A more robust example would do an async validation against the official list at
  // https://www.iso.org/obp/ui/#search/code/
  let make =
    fun
    | str when str |> String.length == 2 => CountryCode(str) |> Option.pure
    | _ => None;

  let toClassName = (CountryCode(str)) => "fi-" ++ str;

  let toString = (CountryCode(str)) => str;
};

module Styles = {
  open Css;

  let wrapper = style([maxWidth(rem(0.875))]);
};

[@react.component]
let make = (~className=?, ~countryCode, ~displayFormat: displayFormat=Normal) =>
  <span
    className={
      className
      |> extendBaseStyle(
           [
             Styles.wrapper,
             displayFormat |> displayFormatToClassName,
             countryCode |> CountryCode.toClassName,
           ]
           |> Css.merge,
         )
    }
  />;
