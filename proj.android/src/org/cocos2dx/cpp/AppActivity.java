/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 ****************************************************************************/
package org.cocos2dx.cpp;

import java.util.List;
import java.util.Set;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.os.Bundle;
import android.util.Log;

import com.easyndk.classes.AndroidNDKHelper;
import com.parse.DeleteCallback;
import com.parse.FindCallback;
import com.parse.GetCallback;
import com.parse.LogInCallback;
import com.parse.Parse;
import com.parse.ParseException;
import com.parse.ParseObject;
import com.parse.ParseQuery;
import com.parse.ParseRelation;
import com.parse.ParseUser;
import com.parse.SaveCallback;
import com.parse.SignUpCallback;

public class AppActivity extends Cocos2dxActivity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		Parse.initialize(this, "n9x5I3pj0cjOWUTMQ9SUJWxXY1JDjyhzuvpHFMta", "h2NEagfONn1W1sLQMMLDz8p1ZzXrSb3QK8mC03IY");
		AndroidNDKHelper.SetNDKReceiver(this);
	}

	/* Your method must have this JSONObject parameter */
	public void nativeTestMethod(JSONObject parameters) {
		if (parameters != null) {
			try {
				String testMessage = parameters.getString("test_message");
				double pi = parameters.getDouble("test_double");

				Log.i("Message received from game", testMessage);
				Log.i("Value of test double", String.valueOf(pi));

				/* Let's send a message back to the game */
				JSONObject message = new JSONObject();
				JSONArray numbersArray = new JSONArray();

				try {
					numbersArray.put(1);
					numbersArray.put(1);
					numbersArray.put(2);
					numbersArray.put(3);
					numbersArray.put(5);
					numbersArray.put(8);

					message.put("test_response", "Greetings from Android!");
					message.put("fibonacci", numbersArray);
				} catch (JSONException e) {
					e.printStackTrace();
				}

				// Send C++ a message with parameters
				// C++ will receive this message only if the selector list has a method
				// of the same name as specified - in this case, "gameTestMethod"
				AndroidNDKHelper.SendMessageWithParameters("gameTestMethod", message);
			} catch (JSONException e) {
				e.printStackTrace();
			}
		}
	}

	public void checkForUser(JSONObject parameters){
		ParseUser currentUser = ParseUser.getCurrentUser();
		if(currentUser != null){
			ParseQuery<ParseUser> query = ParseUser.getQuery();
			query.whereEqualTo("username", currentUser.getUsername());
			query.findInBackground(new FindCallback<ParseUser>() {
				  public void done(List<ParseUser> objects, ParseException e) {
				    if (e == null) {
				    	if(objects.size() > 0){
				    		JSONObject message = new JSONObject();
				    		try {
								message.put("responce", "true");
							} catch (JSONException e1) {
								e1.printStackTrace();
							}
				    		AndroidNDKHelper.SendMessageWithParameters("userCheckCallback", message);
				    	} else {
				    		JSONObject message = new JSONObject();
				    		try {
								message.put("responce", "false");
							} catch (JSONException e1) {
								e1.printStackTrace();
							}
				    		AndroidNDKHelper.SendMessageWithParameters("userCheckCallback", message);
				    	}
				    } else {
				        // Something went wrong.
				    }
				  }
			});
		} else {
			JSONObject message = new JSONObject();
			try {
				message.put("responce", "false");
			} catch (JSONException e) {
				e.printStackTrace();
			}
			AndroidNDKHelper.SendMessageWithParameters("userCheckCallback", message);
		}
	}
	public void register(final JSONObject parameters){
		if (parameters != null) {
			try {
				String username = parameters.getString("username");
				String password = parameters.getString("password");
				ParseUser user = ParseUser.getCurrentUser();
				user.setPassword(password);
				user.setUsername(username);
				user.signUpInBackground(new SignUpCallback() {
					  public void done(ParseException e) {
					    if (e == null) {
					    	logIn(parameters);
					    } else {
				    		JSONObject message = new JSONObject();
				    		try {
								message.put("responce", e.toString());
							} catch (JSONException e1) {
								e1.printStackTrace();
							}
				    		AndroidNDKHelper.SendMessageWithParameters("registerCallback", message);
					    }
					  }
				});
			} catch (JSONException e) {
				e.printStackTrace();
			}
		}
	}
	public void logIn(JSONObject parameters){
		if(parameters != null){
			try{
				String username = parameters.getString("username");
				String password = parameters.getString("password");
				ParseUser.logInInBackground(username, password, new LogInCallback() {
					  public void done(ParseUser user, ParseException e) {
					    if (user != null) {
				    		JSONObject message = new JSONObject();
				    		try {
								message.put("success", "true");
								message.put("responce", "none");
							} catch (JSONException e1) {
								e1.printStackTrace();
							}
				    		AndroidNDKHelper.SendMessageWithParameters("logInCallback", message);
				    	} else {
				    		JSONObject message = new JSONObject();
				    		try {
								message.put("success", "false");
								message.put("responce", e.toString());
							} catch (JSONException e1) {
								e1.printStackTrace();
							}
				    		AndroidNDKHelper.SendMessageWithParameters("logInCallback", message);					    }
					  }
				});
			} catch (JSONException e){
				e.printStackTrace();
			}
		}
	}
	public void newLevel(JSONObject parameters){
		final ParseObject level = new ParseObject("Level");
		level.add("name",  "qq36q81q");
		level.add("author", ParseUser.getCurrentUser().getUsername());
		level.add("entcount", "0");
		level.add("status", "Private");
		level.add("favorites", 0);
		level.saveInBackground(new SaveCallback() {
			public void done(ParseException e) {
				if(e == null){
					level.add("id", level.getObjectId());
					level.saveInBackground(new SaveCallback() {
						public void done(ParseException e) {
							if(e == null){
					    		JSONObject message = new JSONObject();
					    		try {
									message.put("responce", "responce");
								} catch (JSONException e1) {
									e1.printStackTrace();
								}
					    		AndroidNDKHelper.SendMessageWithParameters("newLevelResponce", message);
							} else {
								//error
							}
						}
					});
				} else {
					//error
				}
			}
		});
	}
	public void saveLevel(final JSONObject parameters){
		if(parameters != null){
			try{
				String levelID = parameters.getString("id");
				ParseQuery<ParseObject> query = ParseQuery.getQuery("Level");
				query.getInBackground(levelID, new GetCallback<ParseObject>() {
				     public void done(ParseObject level, ParseException e) {
				         if (e == null) {
				        	 int count = 0;
				        	 try {
								count = Integer.parseInt(parameters.getString("entcount"));
							} catch (NumberFormatException e1) {
								e1.printStackTrace();
							} catch (JSONException e1) {
								e1.printStackTrace();
							}
				        	String tempEntCount = "0";
							String tempName = "name";
							String tempStatus = "Private";
				        	try {
								tempEntCount = parameters.getString("entcount");
								tempName = parameters.getString("name");
								tempStatus = parameters.getString("status");
							} catch (JSONException e1) {
								e1.printStackTrace();
							}
				        	level.add("entcount", tempEntCount);
				        	level.add("status", tempStatus);
				        	level.add("name", tempName);
							JSONArray ents = new JSONArray();
							for(int x = 0; x < count + 1; x++){
								try{
									ents.put(parameters.getString("entity" + x + "type"));
									ents.put(parameters.getString("entity" + x + "x"));
									ents.put(parameters.getString("entity" + x + "y"));
									ents.put(parameters.getString("entity" + x + "width"));
									ents.put(parameters.getString("entity" + x + "height"));
									ents.put(parameters.getString("entity" + x + "velocityX"));
									ents.put(parameters.getString("entity" + x + "velocityY"));
								} catch (JSONException c) {
									c.printStackTrace();
								}
							}
							level.add("entities", ents);
							level.saveInBackground();
				         } else {
				        	 
				         }
				     }
				});
			} catch (JSONException e){
				e.printStackTrace();
			}
		}
	}
	public void saveLevelInfo(final JSONObject parameters){
		if(parameters != null){
			String levelID = "0";
			try {
				levelID = parameters.getString("id");
			} catch (JSONException e) {
				e.printStackTrace();
			}
			ParseQuery<ParseObject> query = ParseQuery.getQuery("Level");
			query.getInBackground(levelID, new GetCallback<ParseObject>() {
			     public void done(ParseObject level, ParseException e) {
			    	 String count = "0";
			    	 String name = "name";
			    	 String status = "Private";
			    	 try {
						count = parameters.getString("entcount");
						name = parameters.getString("name");
						status = parameters.getString("status");
					} catch (JSONException e1) {
						e1.printStackTrace();
					}
		        	 level.add("entcount", count);
		        	 level.add("name",  name);
		        	 level.add("status", status);
		        	 level.saveInBackground();
			     }
			});
		}
	}
	public void deleteLevel(final JSONObject parameters){
		if(parameters != null){
			String levelID = "0";
			try {
				levelID = parameters.getString("id");
			} catch (JSONException e) {
				e.printStackTrace();
			}
			ParseQuery<ParseObject> query = ParseQuery.getQuery("Level");
			query.getInBackground(levelID, new GetCallback<ParseObject>() {
			     public void done(ParseObject level, ParseException e) {
			    	 if(level != null){
			    		 level.deleteInBackground(new DeleteCallback() {
			    			   public void done(ParseException e) {
			 			 			int refresh = 0;
									try {
										refresh = Integer.parseInt(parameters.getString("refresh"));
									} catch (JSONException c) {
										e.printStackTrace();
									}
									if(refresh == 1){
							    		JSONObject message = new JSONObject();
							    		try {
											message.put("responce", "false");
										} catch (JSONException e1) {
											e1.printStackTrace();
										}
							    		AndroidNDKHelper.SendMessageWithParameters("levelDeletedCallback", message);
									} else {
							    		JSONObject message = new JSONObject();
							    		try {
											message.put("responce", "false");
										} catch (JSONException e1) {
											e1.printStackTrace();
										}
							    		AndroidNDKHelper.SendMessageWithParameters("finishQuit", message);
									}
			    			   }
			    			 });
			    	 } else {
			 			int refresh = 0;
						try {
							refresh = Integer.parseInt(parameters.getString("refresh"));
						} catch (JSONException c) {
							e.printStackTrace();
						}
						if(refresh == 1){
				    		JSONObject message = new JSONObject();
				    		try {
								message.put("responce", "false");
							} catch (JSONException e1) {
								e1.printStackTrace();
							}
				    		AndroidNDKHelper.SendMessageWithParameters("levelDeletedCallback", message);
						} else {
				    		JSONObject message = new JSONObject();
				    		try {
								message.put("responce", "false");
							} catch (JSONException e1) {
								e1.printStackTrace();
							}
				    		AndroidNDKHelper.SendMessageWithParameters("finishQuit", message);
						}
			    	 }
			     }
			});
		}
	}
	public void favorite(final JSONObject parameters){
		if(parameters != null){
			String levelID = "0";
			try {
				levelID = parameters.getString("id");
			} catch (JSONException e) {
				e.printStackTrace();
			}
			ParseQuery<ParseObject> query = ParseQuery.getQuery("Level");
			query.getInBackground(levelID, new GetCallback<ParseObject>() {
			     public void done(ParseObject level, ParseException e) {
			    	 int favorites = 0;
			    	 try {
						favorites = Integer.parseInt(parameters.getString("entcount"));
					} catch (JSONException e1) {
						e1.printStackTrace();
					}
		        	 level.add("favorites", favorites + 1);
		        	 level.saveInBackground();
		        	 ParseUser user = ParseUser.getCurrentUser();
		        	 ParseRelation<ParseObject> relation = user.getRelation("favorites");
		        	 relation.add(level);
		        	 user.saveInBackground();
			    		JSONObject message = new JSONObject();
			    		try {
							message.put("responce", "false");
						} catch (JSONException e1) {
							e1.printStackTrace();
						}
			    		AndroidNDKHelper.SendMessageWithParameters("favCallback", message);
			     }
			});
		}
	}
	public void fetchCustomLevels(JSONObject parameters){
		ParseQuery<ParseObject> query = ParseQuery.getQuery("Level");
		query.whereEqualTo("author",  ParseUser.getCurrentUser().getUsername());
		query.findInBackground(new FindCallback<ParseObject>() {
		    public void done(final List<ParseObject> levels, ParseException e) {
		        if (e == null) {
		        	ParseUser user = ParseUser.getCurrentUser();
		        	ParseRelation<ParseObject> relation = user.getRelation("favorites");
		        	relation.getQuery().findInBackground( new FindCallback<ParseObject>() {
		        		public void done(List<ParseObject> favs, ParseException f){
		        			if(f == null){
		    		        	for(int x = 0; x < levels.size(); x++){
		    		        		ParseObject level = levels.get(x);
		    		        		JSONObject dict = new JSONObject();
		    						Set<String> ents = level.keySet();
		    						for(String s : ents){
		    							try {
		    								if(s.equals("entities")){
		    									dict.put(s, level.getJSONArray(s));
		    								} else {
		    									dict.put(s, level.get(s));
		    								}
		    								} catch (JSONException e1) {
		    								// TODO Auto-generated catch block
		    								e1.printStackTrace();
		    							}
		    						}
		    						try {
		    							dict.put("favorited", "false");
		    						} catch (JSONException e1) {
		    							// TODO Auto-generated catch block
		    							e1.printStackTrace();
		    						}
		    						for(ParseObject levelID : favs){
		    							String levelID1 = level.getString("id");
		    							String levelID2 = levelID.getString("id");
		    							if(levelID1.equals(levelID2)){
		    								try {
												dict.put("favorited", "true");
											} catch (JSONException e) {
												// TODO Auto-generated catch block
												e.printStackTrace();
											}
		    								break;
		    							}
		    						}
		    			    		AndroidNDKHelper.SendMessageWithParameters("fetchCustomCallback", dict);
		    		        	}
					    		JSONObject message = new JSONObject();
					    		try {
									message.put("responce", "false");
								} catch (JSONException e1) {
									e1.printStackTrace();
								}
					    		AndroidNDKHelper.SendMessageWithParameters("doneFetching", message);
		        			} else {
		        				
		        			}
		        		}
		        	});
		        } else {
		        	//Error
		        }
		    }
		});
	}
	public void fetchRisingLevels(JSONObject parameters){
		ParseQuery<ParseObject> query = ParseQuery.getQuery("Level");
		query.addDescendingOrder("favorites");
		query.setLimit(12);
		System.out.println("Fetching Rising Levels");
		query.findInBackground(new FindCallback<ParseObject>() {
		    public void done(final List<ParseObject> levels, ParseException e) {
		        if (e == null) {
		        	ParseUser user = ParseUser.getCurrentUser();
		        	ParseRelation<ParseObject> relation = user.getRelation("favorites");
		    		System.out.println("\tAbout to search for favorited Levels");
		        	relation.getQuery().findInBackground( new FindCallback<ParseObject>() {
		        		public void done(List<ParseObject> favs, ParseException f){
		        			if(f == null){
		    		    		System.out.println("\tIterating through levels");
		    		        	for(int x = 0; x < levels.size(); x++){
		    		        		System.out.println("\t\tFound Level");
		    		        		ParseObject level = levels.get(x);
		    		        		JSONObject dict = new JSONObject();
		    						Set<String> keys = level.keySet();
		    						for(String s : keys){
			    						System.out.println("\t\t\tAdding: " + s);
		    							try {
		    								if(s.equals("entities")){
		    									dict.put(s, level.getJSONArray(s));
		    								} else {
		    									dict.put(s, level.get(s));
		    								}
		    							} catch (JSONException e1) {
		    								// TODO Auto-generated catch block
		    								e1.printStackTrace();
		    							}
		    						}
		    						try {
		    							dict.put("favorited", "false");
		    						} catch (JSONException e1) {
		    							// TODO Auto-generated catch block
		    							e1.printStackTrace();
		    						}
		    						for(ParseObject levelID : favs){
		    							String levelID1 = level.getString("id");
		    							String levelID2 = levelID.getString("id");
		    							if(levelID1.equals(levelID2)){
		    								try {
												dict.put("favorited", "true");
											} catch (JSONException e) {
												// TODO Auto-generated catch block
												e.printStackTrace();
											}
		    								break;
		    							}
		    						}
		    			    		System.out.println("\t\t\tAbout to send level");
		    			    		AndroidNDKHelper.SendMessageWithParameters("fetchRisingCallback", dict);
		    		        	}
					    		JSONObject message = new JSONObject();
					    		try {
									message.put("responce", "false");
								} catch (JSONException e1) {
									e1.printStackTrace();
								}
					    		System.out.println("\tAbout to send done fetching");
					    		AndroidNDKHelper.SendMessageWithParameters("doneFetching", message);
		        			} else {
		        				
		        			}
		        		}
		        	});
		        } else {
		        	//Error
		        }
		    }
		});
	}
	public void fetchSocialLevels(JSONObject parameters){
		
	}
	public void fetchFavoritedLevels(JSONObject parameters){
		        	ParseUser user = ParseUser.getCurrentUser();
		        	ParseRelation<ParseObject> relation = user.getRelation("favorites");
		        	relation.getQuery().findInBackground( new FindCallback<ParseObject>() {
		        		public void done(List<ParseObject> levels, ParseException f){
		        			if(f == null){
		    		        	for(int x = 0; x < levels.size(); x++){
		    		        		ParseObject level = levels.get(x);
		    		        		JSONObject dict = new JSONObject();
		    						Set<String> ents = level.keySet();
		    						for(String s : ents){
		    							try {
		    								if(s.equals("entities")){
		    									dict.put(s, level.getJSONArray(s));
		    								} else {
		    									dict.put(s, level.get(s));
		    								}
		    							} catch (JSONException e1) {
		    								// TODO Auto-generated catch block
		    								e1.printStackTrace();
		    							}
		    						}
		    						try {
		    							dict.put("favorited", "true");
		    						} catch (JSONException e1) {
		    							// TODO Auto-generated catch block
		    							e1.printStackTrace();
		    						}
		    			    		AndroidNDKHelper.SendMessageWithParameters("fetchFavoritedCallback", dict);
		    		        	}
					    		JSONObject message = new JSONObject();
					    		try {
									message.put("responce", "false");
								} catch (JSONException e1) {
									e1.printStackTrace();
								}
					    		AndroidNDKHelper.SendMessageWithParameters("doneFetching", message);
		        			} else {
		        				
		        			}
		        		}
		        	});
	}
}