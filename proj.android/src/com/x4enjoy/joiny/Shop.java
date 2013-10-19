package com.x4enjoy.joiny;

import com.soomla.store.IStoreAssets;
import com.soomla.store.domain.GoogleMarketItem;
import com.soomla.store.domain.NonConsumableItem;
import com.soomla.store.domain.VirtualCategory;
import com.soomla.store.domain.virtualCurrencies.VirtualCurrency;
import com.soomla.store.domain.virtualCurrencies.VirtualCurrencyPack;
import com.soomla.store.domain.virtualGoods.SingleUseVG;
import com.soomla.store.domain.virtualGoods.VirtualGood;
import com.soomla.store.purchaseTypes.PurchaseWithMarket;

public class Shop implements IStoreAssets {
	@Override
	public int getVersion() {
		return 0;
	}

	@Override
	public VirtualCurrency[] getCurrencies() {
		return new VirtualCurrency[] {

		};
	}

	@Override
	public VirtualGood[] getGoods() {
		return new VirtualGood[] { T10_HINT_GOOD, T50_HINT_GOOD,
				T1000_HINT_GOOD };
	}

	@Override
	public VirtualCurrencyPack[] getCurrencyPacks() {
		return new VirtualCurrencyPack[] {

		};
	}

	@Override
	public VirtualCategory[] getCategories() {
		return new VirtualCategory[] {

		};
	}

	@Override
	public NonConsumableItem[] getNonConsumableItems() {
		return new NonConsumableItem[] { UNLOCK_ALL_COLLECTIONS,
				BUY_ALL_COLLECTIONS, BUY_COLL_1000, BUY_COLL_2000,
				BUY_COLL_3000, BUY_COLL_4000 };
	}

	public static final String UNLOCK_ALL_COLLECTIONS_ID = "unlock_all";
	public static final String BUY_ALL_COLLECTIONS_ID = "buy_all";
	public static final String BUY_COLL_1000_ID = "buy_1000";
	public static final String BUY_COLL_2000_ID = "buy_2000";
	public static final String BUY_COLL_3000_ID = "buy_3000";
	public static final String BUY_COLL_4000_ID = "buy_4000";
	
	public static final String T10_HINT_GOOD_ID = "hint_10";
	public static final String T50_HINT_GOOD_ID = "hint_50";
	public static final String T1000_HINT_GOOD_ID = "hint_1000";
	
	public static final NonConsumableItem UNLOCK_ALL_COLLECTIONS = new NonConsumableItem(
			App.getStringRes(R.string.purchase_unlock_all_collections_title),
			App.getStringRes(R.string.purchase_unlock_all_collections_description),
			UNLOCK_ALL_COLLECTIONS_ID, new PurchaseWithMarket(
					new GoogleMarketItem(UNLOCK_ALL_COLLECTIONS_ID,
							GoogleMarketItem.Managed.MANAGED, 0.99)));
	
	public static final NonConsumableItem BUY_ALL_COLLECTIONS = new NonConsumableItem(
			App.getStringRes(R.string.purchase_buy_all_collections_title),
			App.getStringRes(R.string.purchase_buy_all_collections_description),
			BUY_ALL_COLLECTIONS_ID, new PurchaseWithMarket(
					new GoogleMarketItem(BUY_ALL_COLLECTIONS_ID,
							GoogleMarketItem.Managed.MANAGED, 1.99)));

	
	public static final NonConsumableItem BUY_COLL_1000 = new NonConsumableItem(
			App.getStringRes(R.string.purchase_buy_one_collection_title),
			App.getStringRes(R.string.purchase_buy_one_collection_description),
			BUY_COLL_1000_ID, new PurchaseWithMarket(
					new GoogleMarketItem(BUY_COLL_1000_ID,
							GoogleMarketItem.Managed.MANAGED, 0.99)));
	
	public static final NonConsumableItem BUY_COLL_2000 = new NonConsumableItem(
			App.getStringRes(R.string.purchase_buy_one_collection_title),
			App.getStringRes(R.string.purchase_buy_one_collection_description),
			BUY_COLL_2000_ID, new PurchaseWithMarket(
					new GoogleMarketItem(BUY_COLL_2000_ID,
							GoogleMarketItem.Managed.MANAGED, 0.99)));
	
	public static final NonConsumableItem BUY_COLL_3000 = new NonConsumableItem(
			App.getStringRes(R.string.purchase_buy_one_collection_title),
			App.getStringRes(R.string.purchase_buy_one_collection_description),
			BUY_COLL_3000_ID, new PurchaseWithMarket(
					new GoogleMarketItem(BUY_COLL_3000_ID,
							GoogleMarketItem.Managed.MANAGED, 0.99)));
	
	public static final NonConsumableItem BUY_COLL_4000 = new NonConsumableItem(
			App.getStringRes(R.string.purchase_buy_one_collection_title),
			App.getStringRes(R.string.purchase_buy_one_collection_description),
			BUY_COLL_4000_ID, new PurchaseWithMarket(
					new GoogleMarketItem(BUY_COLL_4000_ID,
							GoogleMarketItem.Managed.MANAGED, 0.99)));
	
	public static final VirtualGood T10_HINT_GOOD = new SingleUseVG(
			App.getStringRes(R.string.purchase_hint_10_title),                                        
			App.getStringRes(R.string.purchase_hint_10_description),   
            T10_HINT_GOOD_ID,                                        
            new PurchaseWithMarket(
					new GoogleMarketItem(T10_HINT_GOOD_ID,
							GoogleMarketItem.Managed.UNMANAGED, 0.99))); 
	
	public static final VirtualGood T50_HINT_GOOD = new SingleUseVG(
			App.getStringRes(R.string.purchase_hint_50_title),                                        
			App.getStringRes(R.string.purchase_hint_50_description),    
            T50_HINT_GOOD_ID,                                        
            new PurchaseWithMarket(
					new GoogleMarketItem(T50_HINT_GOOD_ID,
							GoogleMarketItem.Managed.UNMANAGED, 1.99))); 
	
	public static final VirtualGood T1000_HINT_GOOD = new SingleUseVG(
			App.getStringRes(R.string.purchase_hint_1000_title),                                        
			App.getStringRes(R.string.purchase_hint_1000_description),  
            T1000_HINT_GOOD_ID,                                        
            new PurchaseWithMarket(
					new GoogleMarketItem(T1000_HINT_GOOD_ID,
							GoogleMarketItem.Managed.UNMANAGED, 4.99))); 

}
