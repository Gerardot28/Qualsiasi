import { useState } from 'react';
import styles from './Header.module.css';

export default function Header({ onSearch, activeTab, onTabChange }) {
  const [query, setQuery] = useState('');

  const handleSubmit = (e) => {
    e.preventDefault();
    if (query.trim()) {
      onSearch(query.trim());
      onTabChange('search');
    }
  };

  const tabs = [
    { id: 'season', label: '🌸 Stagione' },
    { id: 'schedule', label: '📅 Palinsesto' },
    { id: 'top', label: '🏆 Top Airing' },
  ];

  return (
    <header className={styles.header}>
      <div className={styles.inner}>
        <div className={styles.brand}>
          <span className={styles.logo}>⛩</span>
          <div>
            <h1 className={styles.title}>AnimeTracker</h1>
            <p className={styles.sub}>Ultime uscite anime in tempo reale</p>
          </div>
        </div>

        <form className={styles.searchForm} onSubmit={handleSubmit}>
          <input
            type="text"
            className={styles.searchInput}
            placeholder="Cerca un anime..."
            value={query}
            onChange={(e) => setQuery(e.target.value)}
          />
          <button type="submit" className={styles.searchBtn}>
            🔍
          </button>
        </form>

        <nav className={styles.nav}>
          {tabs.map((t) => (
            <button
              key={t.id}
              className={`${styles.tab} ${activeTab === t.id ? styles.tabActive : ''}`}
              onClick={() => onTabChange(t.id)}
            >
              {t.label}
            </button>
          ))}
          {activeTab === 'search' && (
            <button className={`${styles.tab} ${styles.tabActive}`}>🔍 Ricerca</button>
          )}
        </nav>
      </div>
    </header>
  );
}
