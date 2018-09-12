<?php

/**
 * Custom Form Test Registration Form.
 *
 * @param $form
 * @param $form_state
 *
 * @return mixed
 */
function custom_form_test_registration_form($form, &$form_state) {

  $form['logo']['contacts'] = [
    '#markup' => '<div class="logo"><a href="/">logo</a></div>',
  ];

  $form['registration'] = [
    '#title' => t('Register for a 14-day Free Trial'),
    '#type' => 'fieldset',
  ];

  $form['registration']['domain'] = [
    '#type' => 'textfield',
    '#title' => t('Domain'),
    '#title_display' => 'invisible',
    '#required' => TRUE,
    '#size' => 20,
    '#prefix' => '<div id="domain-ajax">',
    '#suffix' => '<div class="suffix-domain">.bpmonline.com</div></div>',
    '#maxlength' => 20,
    '#attributes' => [
      'placeholder' => t('Domain*'),
      'title' => t('Please enter your domain name'),
    ],
    '#ajax' => [
      'callback' => 'custom_form_test_registration_form_domain_exist_ajax',
      'wrapper' => 'domain-ajax',
      'event' => 'blur',
    ],
  ];

  $form['registration']['login'] = [
    '#type' => 'textfield',
    '#title' => t('Login'),
    '#title_display' => 'invisible',
    '#required' => TRUE,
    '#size' => 20,
    '#maxlength' => 15,
    '#attributes' => [
      'placeholder' => t('Login*'),
      'title' => t('Please enter your login name'),
    ],
  ];
// Get list of all predefined and custom countries
  $country_list = country_get_list();

  $country_list = ['' => t('Country*')] + $country_list;

  $form['registration']['country'] = [
    '#type' => 'select',
    '#title' => t('Country'),
    '#title_display' => 'invisible',
    '#required' => TRUE,
    '#options' => $country_list,
    '#default_value' => key($country_list),
    '#attributes' => [
      'title' => t('Please choose your country'),
    ],
  ];

  $form['registration']['city'] = [
    '#type' => 'textfield',
    '#title' => t('City'),
    '#title_display' => 'invisible',
    '#size' => 20,
    '#maxlength' => 20,
    '#attributes' => [
      'placeholder' => t('City'),
      'title' => t('Please enter your city'),
    ],
  ];

  $form['registration']['code'] = [
    '#type' => 'textfield',
    '#title' => t('Code'),
    '#title_display' => 'invisible',
    '#required' => TRUE,
    '#size' => 20,
    '#maxlength' => 5,
    '#attributes' => [
      'placeholder' => t('Code*'),
      'title' => t('Please enter your code'),
    ],
  ];
  $form['registration']['phone'] = [
    '#type' => 'textfield',
    '#title' => t('Phone'),
    '#title_display' => 'invisible',
    '#required' => TRUE,
    '#size' => 20,
    '#maxlength' => 9,
    '#attributes' => [
      'placeholder' => t('Phone*'),
      'title' => t('Please enter your phone'),
    ],
  ];

  $form['registration']['submit'] = [
    '#type' => 'submit',
    '#value' => t('Register'),
  ];

  $form['registration']['create_account'] = [
    '#markup' => '<div class="create-account"><a href="/">' . t('Create Accounts') . '</a></div>',
  ];

  $form['link']['main_page'] = [
    '#markup' => '<div class="main-page"><a href="/">' . t('Main Page') . '</a></div>',
  ];

  $form['link']['contacts'] = [
    '#markup' => '<div class="contacts"><a href="/">' . t('Contacts') . '</a></div>',
  ];
  return $form;
}


/**
 * Check if domain name not taken
 *
 * @param $form
 * @param $form_state
 * @return mixed
 */
function custom_form_test_registration_form_domain_exist_ajax(&$form, &$form_state) {
  $domain = $form_state['values']['domain'];

  $sql = db_select('registration_form', 'n')
    ->fields('n', ['domain'])
    ->condition('n.domain', $domain, '=');
  $result = $sql->execute()->fetchAssoc();
  if (isset($result['domain'])) {
    form_set_error('domain', t('The domain has already taken'));
  }
  return $form['registration']['domain'];
}

/**
 * Validate callback for Custom Form Test Registration Form.
 *
 * @param $form
 * @param $form_state
 */
function custom_form_test_registration_form_validate($form, $form_state) {
  $domain = $form_state['values']['domain'];
  $login = $form_state['values']['login'];
  $country = $form_state['values']['country'];
  $city = $form_state['values']['city'];
  $code = $form_state['values']['code'];
  $phone = $form_state['values']['phone'];

  if (!empty($domain) && !preg_match('/^[a-z0-9.\-]+$/', $domain)) {
    form_set_error('domain', t('The domain name must be only numbers or letters'));
  }
  if (custom_form_test_registration_form_domain_exist_validation($domain)) {
    form_set_error('domain', t('Domain already exists'));
  }

  if (!empty($login) && !preg_match('/^[a-z_-]{3,15}$/', $login)) {
    form_set_error('login', t('Incorrect Login'));
  }

  if (empty($country)) {
    form_set_error('country', t('Incorrect country'));
  }

  if (!empty($city) && !preg_match('/^(?:[a-zA-Z]+(?:[.\'\-,])?\s?)+$/', $city)) {
    form_set_error('city', t('Incorrect city'));
  }

  if (!empty($code) && !preg_match('/^\(\d{3}\)?$/', $code)) {
    form_set_error('code', t('Incorrect code'));
  }

  if (!empty($phone) && !preg_match('/^\d{3}\-\d{2}\-\d{2}?$/', $phone)) {
    form_set_error('phone', t('Incorrect phone'));
  }
}

/**
 * Validate if domain name not taken.
 *
 * @param $domain
 * @return bool
 */
function custom_form_test_registration_form_domain_exist_validation($domain) {

  $sql = db_select('registration_form', 'n')
    ->fields('n', ['domain'])
    ->condition('n.domain', $domain, '=');
  $result = $sql->execute()->fetchAssoc();
  if (isset($result['domain'])) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Submit callback for Custom Form Test Registration Form.
 *
 * @param $form
 * @param $form_state
 */
function custom_form_test_registration_form_submit($form, $form_state) {
  $values = [
    'domain' => $form_state['values']['domain'],
    'login' => $form_state['values']['login'],
    'country' => $form_state['values']['country'],
    'city' => !empty($form_state['values']['city']) ? $form_state['values']['city'] : null,
    'code' => $form_state['values']['code'],
    'phone' => $form_state['values']['phone'],
  ];

  $insert = db_insert('registration_form')
    ->fields([
      'domain' => $values['domain'],
      'login' => $values['login'],
      'country' => $values['country'],
      'city' => $values['city'],
      'phone' => $values['code'] . ' ' . $values['phone'],
    ])
    ->execute();

  $body[] = 'Hello,';
  $to = 'test@test.com';
  $params = [
    'body' => $body,
    'subject' => 'Test letter',
  ];
  if (drupal_mail('custom_form_test', 'custom_form_test_mail', $to, language_default(), $params)) {
    drupal_set_message('Thanks, email sent.');
  }
  else {
    drupal_set_message('Email not sent.');
  }
}

/**
 * Implementation of hook_mail()
 */
function custom_form_test_mail($key, &$message, $params) {
  $message['subject'] = $params['subject'];
  $message['body'] = $params['body'];
}