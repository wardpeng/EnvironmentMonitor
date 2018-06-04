package com.sap.ward.ai.model;

import com.fasterxml.jackson.annotation.JsonProperty;

import lombok.Data;

@Data
public class TulingResponse
{
	@JsonProperty("code")
	private long code;

	@JsonProperty("text")
	private String text;

}
