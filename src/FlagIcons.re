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

  let make =
    fun
    | str when str |> String.length == 2 => CountryCode(str) |> Option.pure
    | _ => None;

  let toClassName = (CountryCode(str)) => "fi-" ++ str;
};

[@react.component]
let make = (~className=?, ~countryCode, ~displayFormat: displayFormat=Normal) =>
  <span
    className={
      [
        displayFormat |> displayFormatToClassName,
        countryCode |> CountryCode.toClassName,
      ]
      |> mergeClassNames(className)
    }
  />;
