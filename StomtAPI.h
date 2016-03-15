// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include<string>




using namespace std;


/**
 * 
 */
class STOMT_API StomtAPI
{

public:

	StomtAPI();
	~StomtAPI();

	void CreateStomt(string target_id);
	void MakeTestRequest();

private:

	string		target_id;

	bool		positive;

	string		text;
	string		url;

	bool		anonym;

	string		img_name;
	string		lonlat;

};
