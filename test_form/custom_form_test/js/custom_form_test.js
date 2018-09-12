(function ($) {
    Drupal.behaviors.custom_form_test = {
        attach: function (context) {

            $('input[name="code"]').keyup(function () {
                if (this.value.length == this.maxLength) {
                    $(this).parent().next().find('input[name="phone"]').focus();
                }
            });

            $('input[name="code"]').mask('(999)', {placeholder: ""});
            $('input[name="phone"]').mask('999-99-99', {placeholder: ""});

            $.validator.addMethod(
                "regex",
                function (value, element, regexp) {
                    var check = false;
                    return this.optional(element) || regexp.test(value);
                },
                "Please check your input."
            );

            $('#custom-form-test-registration-form').validate({
                rules: {
                    city: {
                        required: false,
                        regex: /^(?:[a-zA-Z]+(?:[.'\-,])?\s?)+$/
                    },
                    login: {
                        required: true,
                        regex: /^[a-z_-]{3,15}$/
                    },
                    domain: {
                        required: true,
                        regex: /^[a-z0-9.\-]+$/
                    },
                    code: {
                        required: true,
                        regex: /^\(\d{3}\)?$/
                    },
                    phone: {
                        required: true,
                        regex: /^\d{3}\-\d{2}\-\d{2}?$/
                    }
                },
                messages: {
                    city: {
                        regex: "Incorrect city",
                    },
                    login: {
                        regex: "Incorrect login",
                    },
                    domain: {
                        regex: "The domain name must be only numbers or letters",
                    },
                    code: {
                        regex: "Code must be only numbers",
                    },
                    phone: {
                        regex: "Phone must be only numbers",
                    },
                },
            });
        }
    }
})(jQuery);