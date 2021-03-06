<?php

/**
 * @file
 * API for Salsa webform integration
 */

class salsaAPI {

	public $email = 'PASS_EMAIL_WHEN_INSTANTIATING_CLASS';
	public $passwd = 'PASS_PASSWORD_WHEN_INSTANTIATING_CLASS';
	public $nodeURL = 'PASS_NODE_URL_WHEN_INSTANTIATING_CLASS';

	public function __construct($nodeURL = null, $email = null, $passwd = null) {
		if(!extension_loaded('curl')) trigger_error('salsaAPI requires PHP cURL', E_USER_ERROR);
		if(is_null($email)) trigger_error('email must be supplied', E_USER_ERROR);
		if(is_null($passwd)) trigger_error('password must be supplied', E_USER_ERROR);
		if(is_null($nodeURL)) trigger_error('Salsa node URL must be supplied', E_USER_ERROR);
		$this->email = $email;
		$this->passwd = $passwd;
		$this->nodeURL = $nodeURL;
	}

	/* general API call functions */

	public function authenticate() {
		$ch = curl_init();

		$authfields["email"] = $this->email;
		$authfields["password"] = $this->passwd;
		$url = $this->nodeURL;

		// Set basic connection parameters:
		curl_setopt($ch, CURLOPT_POST, 1);
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
		curl_setopt($ch, CURLOPT_TIMEOUT, 100);

		// Set Parameters to maintain cookies across sessions
		curl_setopt($ch, CURLOPT_COOKIESESSION, TRUE);
		curl_setopt($ch, CURLOPT_COOKIEFILE, '/tmp/cookies_file');
		curl_setopt($ch, CURLOPT_COOKIEJAR, '/tmp/cookies_file');

		curl_setopt($ch, CURLOPT_URL, "$url/api/authenticate.sjs");
		curl_setopt($ch, CURLOPT_POSTFIELDS, html_entity_decode(http_build_query($authfields)));
		$auth = curl_exec($ch);

		curl_close($ch);
		return $auth;
	}

	public function call($method, $fields, $salsaGroups) {
		$ch = curl_init();

		$url = $this->nodeURL;

		// Set basic connection parameters:
		curl_setopt($ch, CURLOPT_POST, 1);
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
		curl_setopt($ch, CURLOPT_TIMEOUT, 100);

		// Set Parameters to maintain cookies across sessions
/*
		curl_setopt($ch, CURLOPT_COOKIESESSION, TRUE);
		curl_setopt($ch, CURLOPT_COOKIEFILE, '/tmp/cookies_file');
		curl_setopt($ch, CURLOPT_COOKIEJAR, '/tmp/cookies_file');
*/

		$groupsQuery = '';
		if (is_array($salsaGroups)) {
			foreach ($salsaGroups as $g) {
				$groupsQuery .= '&link=groups&linkKey='.$g;
			}
		}

		$query = html_entity_decode(http_build_query($fields));
		if (isset($fields['action_KEY'])) {
			$query .= '&link=action&linkKey='.$fields['action_KEY'];
		}
		$query .= $groupsQuery;
		// mail('jk@webskillet.com','webform salsa query',$query,"From: Firestick Dev <dev@firestick.me>\r\n");
		curl_setopt($ch, CURLOPT_POSTFIELDS, $query);
		curl_setopt($ch, CURLOPT_URL, "$url/$method");
		$response = curl_exec($ch);

		curl_close($ch);
		return $returnraw ? $reponse : $this->response_to_xml($response);
	}

	private function response_to_xml($response) {
		$xml = simplexml_load_string($response);
		return $xml;
	}

}


