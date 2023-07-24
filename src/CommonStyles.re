open Css;

module Colors = {
  module Light = {
    module Background = {
      let box = hex("FFFFFF");
      let selected = hex("FFDBB3");
    };

    module Border = {
      let controlAlpha = (alpha: float) => `rgba((0, 0, 0, `num(alpha)));
      let lineAlpha = (alpha: float) => `rgba((0, 0, 0, `num(alpha)));
    };

    module Text = {
      let primary = hex("333333");
      let secondary = hex("000000");
      let tertiary = hex("000000");

      let secondaryWithAlpha = (alpha: float) =>
        `rgba((0, 0, 0, `num(alpha)));
      let tertiaryWithAlpha = (alpha: float) =>
        `rgba((0, 0, 0, `num(alpha)));
    };
  };
};

module Text = {
  let defaultStyles =
    style([
      color(Colors.Light.Text.primary),
      fontFamily(`custom("Arial")),
      fontSize(rem(0.875)),
      fontStyle(normal),
      fontWeight(`num(400)),
      lineHeight(rem(1.125)),
    ]);
};
