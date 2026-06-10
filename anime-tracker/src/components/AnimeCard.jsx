import styles from './AnimeCard.module.css';

const RATING_COLORS = {
  G: '#4ade80',
  PG: '#60a5fa',
  'PG-13': '#facc15',
  R: '#f87171',
  'R+': '#f97316',
  Rx: '#a855f7',
};

export default function AnimeCard({ anime, onClick }) {
  const score = anime.score?.toFixed(1) ?? '—';
  const episodes = anime.episodes ?? '?';
  const status = anime.status;
  const rating = anime.rating?.split(' ')[0];
  const ratingColor = RATING_COLORS[rating] || '#94a3b8';

  return (
    <div className={styles.card} onClick={() => onClick(anime)}>
      <div className={styles.imageWrapper}>
        <img
          src={anime.images?.jpg?.large_image_url || anime.images?.jpg?.image_url}
          alt={anime.title}
          loading="lazy"
        />
        {score !== '—' && (
          <span className={styles.score}>
            <span className={styles.star}>★</span> {score}
          </span>
        )}
        {rating && (
          <span className={styles.rating} style={{ borderColor: ratingColor, color: ratingColor }}>
            {rating}
          </span>
        )}
      </div>
      <div className={styles.info}>
        <h3 className={styles.title} title={anime.title}>
          {anime.title}
        </h3>
        {anime.title_english && anime.title_english !== anime.title && (
          <p className={styles.titleEn}>{anime.title_english}</p>
        )}
        <div className={styles.meta}>
          <span className={styles.type}>{anime.type || 'TV'}</span>
          <span className={styles.eps}>{episodes} ep</span>
          {status === 'Currently Airing' && <span className={styles.airing}>● IN ONDA</span>}
        </div>
        {anime.genres?.length > 0 && (
          <div className={styles.genres}>
            {anime.genres.slice(0, 3).map((g) => (
              <span key={g.mal_id} className={styles.genre}>
                {g.name}
              </span>
            ))}
          </div>
        )}
      </div>
    </div>
  );
}
