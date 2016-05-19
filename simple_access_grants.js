(function ($) {

  Backdrop.behaviors.simple_accessFieldsetSummaries = {
    attach: function (context) {
      $('fieldset#edit-simple_access').backdropSetSummary(function (context) {
        if (!$('.form-checkbox:checked', context).size()) {
          return Backdrop.t('Public access');
        }
        else {
          return Backdrop.t('Restricted access');
        }
      });
    }
  };

})(jQuery);
